#include "N64_gamepad.h"

namespace gamepad {

#define N64_PIN_DIR DDRD
#define N64_PIN_PIN PIND

#define N64_PIN_HIGH(pinbit) ((N64_PIN_DIR &= ~((pinbit))))
#define N64_PIN_LOW(pinbit)   ((N64_PIN_DIR |= ((pinbit))))
#define N64_PIN_QUERY(pinbit)  ((N64_PIN_PIN & ((pinbit))))

#define N64_PIN_INDEX N64_pin
#define N64_PIN_BIT   N64_pin_bit

#define N64_HIGH  N64_PIN_HIGH ((N64_PIN_BIT))
#define N64_LOW   N64_PIN_LOW  ((N64_PIN_BIT))
#define N64_QUERY N64_PIN_QUERY((N64_PIN_BIT))


static const String N64_gamepad::names[] = {
  // 0           1           2     3         4       5         6         7
    "A",        "B",        "Z",  "start",  "Dup",  "Ddown",  "Dleft",  "Dright",
  // 8           9           10    11        12      13        14        15
    "reset",    "unkown",   "L",  "R",      "Cup",  "Cdown",  "Cleft",  "Cright" };
  // reset is a virtual pin when pressing start+R+L(at same time)
  //       this will report both R and L pressed(1), but start will report unpressed(0)
  //          ^ watch out this for your application
  //            (this is normal N64 console/controller operation)
  //       AND this will reset controller analog position, as if console has started
  //          ^ the behavior is that zero position is considered from where the stick
  //            was when console started or controller reseted (N64 analog is differential)


void N64_gamepad::AndrewBrownInitialize()
{
  // don't remove these lines, we don't want to push +5V to the controller
  digitalWrite(N64_pin, LOW);
  pinMode(N64_pin, INPUT);

  // this is unnecessary for a standard controller, but is required for the Wavebird.
  unsigned char initialize = 0x00;
  noInterrupts();
  AndrewBrownSend(&initialize, 1);
  
  // controller response
  for (uint8_t ii = 0; ii < 64; ii++) {
    // make sure the line is idle for 64 iterations, should be plenty.
    if (!N64_QUERY) ii = 0;
  }
}

void N64_gamepad::AndrewBrownSend(unsigned char *buffer, char length)
{
  // Send these bytes
  char bits;
  bool bit;

  // This routine is very carefully timed by examining the assembly output.
  // Do not change any statements, it could throw the timings off
  //
  // We get 16 cycles per microsecond, which should be plenty, but we need to
  // be conservative. Most assembly ops take 1 cycle, but a few take 2
  //
  // I use manually constructed for-loops out of gotos so I have more control
  // over the outputted assembly. I can insert nops where it was impossible
  // with a for loop

  asm volatile (";Starting outer for loop");
  outer_loop:
  {
    asm volatile (";Starting inner for loop");
    bits = 8;
    inner_loop:
    {
      // Starting a bit, set the line low
      asm volatile (";Setting line to low");
      N64_LOW; // 1 op, 2 cycles

      asm volatile (";branching");
      if (*buffer >> 7) {
        asm volatile (";Bit is a 1");
        // 1 bit
        // remain low for 1us, then go high for 3us
        asm volatile ("nop\nnop\nnop\nnop\nnop\n");
        asm volatile (";Setting line to high");
        N64_HIGH;
        // we'll wait only 2us to sync up with both conditions
        // at the bottom of the if statement
        asm volatile ("nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
                      "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n");

      } else {
        asm volatile (";Bit is a 0");
        // 0 bit
        // remain low for 3us, then go high for 1us
        asm volatile ("nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
                      "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
                      "nop\nnop\nnop\nnop\nnop\nnop\n");
        asm volatile (";Setting line to high");
        N64_HIGH;
        // wait for 1us
        asm volatile ("; end of conditional branch, need to wait 1us more before next bit");
      }
      // end of the if, the line is high and needs to remain
      // high for exactly 16 more cycles, regardless of the previous
      // branch path
      asm volatile (";finishing inner loop body");
      --bits;
      if (bits != 0) {
        // this block is why a for loop was impossible
        asm volatile ("nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n");
        // rotate bits
        asm volatile (";rotating out bits");
        *buffer <<= 1;

        goto inner_loop;
      } // fall out of inner loop
    }
    asm volatile (";continuing outer loop");
    // In this case: the inner loop exits and the outer loop iterates,
    // there are /exactly/ 16 cycles taken up by the necessary operations.
    // So no nops are needed here (that was lucky!)
    --length;
    if (length != 0) {
      ++buffer;
      goto outer_loop;
    } // fall out of outer loop
  }
  // send a single stop (1) bit
  asm volatile ("nop\nnop\nnop\nnop\n");
  N64_LOW;
  // wait 1 us, 16 cycles, then raise the line 16-2=14
  asm volatile ("nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n");
  N64_HIGH;
}

void N64_gamepad::AndrewBrownGet()
{
  // listen for the expected 8 bytes of data back from the controller and
  // blast it out to the N64_raw_dump array, one bit per byte for extra speed.
  // Afterwards, call translate_raw_data() to interpret the raw data and pack
  // it into the N64_status struct.
  asm volatile (";Starting to listen");
  unsigned char timeout;
  char bitcount = 32;
  char *bitbin = raw_dump;

  // Again, using gotos here to make the assembly more predictable and
  // optimization easier (please don't kill me)
  read_loop:
    timeout = 0x3f;
    // wait for line to go low
    while (N64_QUERY) {
      if (!--timeout)
        return;
    }
    // wait approx 2us and poll the line
    asm volatile (
      "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
      "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n"
      "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n");
    *bitbin = N64_QUERY;
    ++bitbin;
    --bitcount;
    if (bitcount == 0)
      return;
  
    // wait for line to go high again it may already be high, so this should just drop through
    timeout = 0x3f;
    while (!N64_QUERY) {
      if (!--timeout)
        return;
    }
    goto read_loop;
}

}