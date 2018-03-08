 # `gamepad`

`[library for arduino-compatible boards]`

`[aka gamepaduino]`

```
interfaces for retro/oldschool gamepads
[game controllers, aka joysticks/joypads/manetes/etc]
```

> _this library is in early stage of development_

> _so please [report bugs](https://github.com/marcosassis/gamepaduino/issues) or feedback me about development and user [experience](https://t.me/joinchat/B4GWUEiFR6LvA_47JwIgQg)_

## `by`

```
marcos assis
metamero.tumblr.com
```

## `now`

`available on gamepad library`

##### `physical controllers interfaces/protocols`

```
[read gamepads/controllers from your arduino-compatible board]
    SNES controller interface [multiplayer]
    N64 controller interface [multiplayer]
```

##### `multiplayer`

```
[read multiple controllers on the fly]
[with cool specialization/optimization for SNES multiplayer protocol]
    optional, turn on by default, easy to turn off
    this module depends on: 'linked list library'
```

##### `'arduino joystick library' adaptor`

```
[go USB HID! make your device appears as joystick/gamepad(s) on your OS(any)]
    for 'pro micro' board[tested], or any ATmega32U4 board, as 'arduino leonardo'
    this optional, just include if (you want and) your board is supported by this library
    this module depends on: 'arduino joystick library'
```

##### `[advanced]`

```
structured interfaces / classes hierarchy
    [template class library]
        robust and meaningful generic interfaces and implementations
        cool specializations / concrete implementations of controllers out-of-the-box
        flexible options (structured parametrization)
        but with meaningful names, and helpful aliases and includes for beginners
            the idea is for this library to be **consistently expanded**, and at the same time,
            **be transparent for beginners**, about this OO/GP internals
```

##### `and most important`

```
examples!
    [with minimal number of commands ever]
        ultra-easy and intuitive *minimalist* interfaces and program flow
        and growing documentation
```

## `docs`

[`wiki`](https://github.com/marcosassis/gamepaduino/wiki)

## `contact`

[`telegram group`](https://t.me/joinchat/B4GWUEiFR6LvA_47JwIgQg) `[pt-br + en]`
