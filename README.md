 # `gamepad`

`[library for arduino-compatible boards]`

`[aka gamepaduino]`

```
interfaces for retro/oldschool gamepads
[game controllers, aka joysticks/joypads/manetes/etc]
```

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
[as default implementation is also ultra lean, N64 multiplayer is as fast as possible too]
    optional, turned on by default, easy to turn off
    this module depends on: 'linked list library'
```

##### `'arduino joystick library' adaptor`

```
[go USB HID! make your device(s) appear as a joystick or gamepad in your OS(any)]
[for 'pro micro' board[tested], or any ATmega32U4 board, such as 'arduino leonardo']
    this is optional, just include it if (you want to and) your board is supported by this library
    this module depends on: 'arduino joystick library'
```

##### `[advanced]`

```
structured interfaces / classes hierarchy
    [template class library]
        robust and meaningful generic interfaces and implementations
        cool specializations / concrete implementations of controllers out-of-the-box
        flexible options (structured parameterization)
        but with meaningful names, and helpful aliases and includes for beginners
            the idea is for this library to be *consistently expanded*, while remaining
            *simple for beginners*, thus it's OO/GP internals
```

##### `and most important`

```
examples!
    [with minimal number of commands ever]
        ultra-easy and intuitive minimalist interfaces and program flow
        and growing documentation
```

## `docs`

[`wiki`](https://github.com/marcosassis/gamepaduino/wiki)

## `contact`

[`telegram group`](https://t.me/joinchat/B4GWUEiFR6LvA_47JwIgQg) `[pt-br + en]`

##### `notes`

<sub>

> 
> _code is [licensed](https://github.com/marcosassis/gamepaduino/blob/master/LICENSE) by (LGPL3) marcos assis 2018_
> 
> 
> _all creative non-code work here is [licensed](https://creativecommons.org/licenses/by-sa/2.0/) by (cc-by-sa) marcos assis 2018_
> 
> 
> _this library is in early stage of development_. 
> _so please [report bugs](https://github.com/marcosassis/gamepaduino/issues) or feedback me about development and user [experience](https://t.me/joinchat/B4GWUEiFR6LvA_47JwIgQg)_.

</sub>

----
