"""
MicroPython max7219 cascadable 8x8 LED matrix driver

Licensed under MIT, found in LICENSE.txt
    Copyright (c) 2017 Mike Causer
    Copyright (c) 2022 Leo Spratt
"""
from micropython import const
from framebuf import FrameBuffer, MONO_HLSB
from time import sleep_ms

_NOOP = const(0)
_DIGIT0 = const(1)
_DECODEMODE = const(9)
_INTENSITY = const(10)
_SCANLIMIT = const(11)
_SHUTDOWN = const(12)
_DISPLAYTEST = const(15)

GLYPHS = {
    "æ": [
        [0, 0, 0, 0, 0, 0, 0, 0],  # ________
        [0, 0, 0, 0, 0, 0, 0, 0],  # ________
        [1, 1, 1, 0, 1, 1, 0, 0],  # ###_##__
        [0, 0, 0, 1, 0, 1, 1, 0],  # ___#_##_
        [0, 1, 1, 1, 1, 1, 1, 0],  # _######_
        [1, 1, 0, 1, 0, 0, 0, 0],  # ##_#____
        [0, 1, 1, 1, 1, 1, 0, 0],  # _#####__
        [0, 0, 0, 0, 0, 0, 0, 0],  # ________
    ],
    "ø": [
        [0, 0, 0, 0, 0, 0, 0, 0],  # ________
        [0, 0, 0, 0, 0, 0, 0, 0],  # ________
        [0, 0, 1, 1, 1, 1, 0, 0],  # __####__
        [0, 1, 1, 0, 0, 1, 1, 0],  # _##__##_
        [0, 1, 1, 0, 1, 1, 1, 0],  # _##_###_
        [0, 1, 1, 1, 0, 1, 1, 0],  # _###_##_
        [0, 0, 1, 1, 1, 1, 0, 0],  # __####__
        [0, 0, 0, 0, 0, 0, 0, 0],  # ________
    ],
    "å": [
        [0, 0, 0, 0, 1, 0, 0, 0], # ____#___
        [0, 0, 0, 0, 0, 0, 0, 0], # ________
        [0, 0, 1, 1, 1, 1, 0, 0], # __####__
        [0, 0, 0, 0, 0, 1, 1, 0], # _____##_
        [0, 0, 1, 1, 1, 1, 1, 0], # __#####_
        [0, 1, 1, 0, 0, 1, 1, 0], # _##__##_
        [0, 0, 1, 1, 1, 1, 1, 0], # __#####_
        [0, 0, 0, 0, 0, 0, 0, 0], # ________
    ],
    "Æ": [
        [0, 0, 1, 1, 1, 1, 1, 0], # __#####_
        [0, 1, 1, 0, 1, 1, 0, 0], # _##_##__
        [0, 1, 1, 0, 1, 1, 0, 0], # _##_##__
        [0, 1, 1, 1, 1, 1, 1, 0], # _######_
        [0, 1, 1, 0, 1, 1, 0, 0], # _##_##__
        [0, 1, 1, 0, 1, 1, 0, 0], # _##_##__
        [0, 1, 1, 0, 1, 1, 1, 0], # _##_###_
        [0, 0, 0, 0, 0, 0, 0, 0], # ________
    ],
    "Ø": [
        [0, 0, 1, 1, 1, 1, 0, 0],  # __####__
        [0, 1, 1, 0, 0, 1, 1, 0],  # _##__##_
        [0, 1, 1, 0, 1, 1, 1, 0],  # _##_###_
        [0, 1, 1, 1, 1, 1, 1, 0],  # _######_
        [0, 1, 1, 1, 0, 1, 1, 0],  # _###_##_
        [0, 1, 1, 0, 0, 1, 1, 0],  # _##__##_
        [0, 0, 1, 1, 1, 1, 0, 0],  # __####__
        [0, 0, 0, 0, 0, 0, 0, 0],  # ________
    ],
    "Å": [
        [0, 0, 0, 1, 1, 0, 0, 0], # ___##___
        [0, 0, 0, 0, 0, 0, 0, 0], # ________
        [0, 0, 0, 1, 1, 0, 0, 0], # ___##___
        [0, 0, 1, 1, 1, 1, 0, 0], # __####__
        [0, 1, 1, 0, 0, 1, 1, 0], # _##__##_
        [0, 1, 1, 1, 1, 1, 1, 0], # _######_
        [0, 1, 1, 0, 0, 1, 1, 0], # _##__##_
        [0, 0, 0, 0, 0, 0, 0, 0], # ________
    ],
}



class Matrix8x8(FrameBuffer):
    def __init__(self, spi, cs, num):
        """
        Driver for cascading MAX7219 8x8 LED matrices.

        >>> from machine import Pin, SPI
        >>> from max7219 import Matrix8x8
        >>> spi = SPI(1)
        >>> display = Matrix8x8(spi, Pin('X5'), 4)
        >>> display.text('1234')
        >>> display.show()

        """
        self._spi = spi
        self._cs = cs
        self._cs.init(self._cs.OUT, True)
        self._num = num
        self._buffer = bytearray(8 * self._num)

        super().__init__(self._buffer, 8 * self._num, 8, MONO_HLSB)

        self._write_init()

    def _write(self, command, data):
        self._cs(0)
        for _ in range(self._num):
            self._spi.write(bytearray([command, data]))
        self._cs(1)

    def _write_init(self):
        for command, data in (
            (_SHUTDOWN, 0),
            (_DISPLAYTEST, 0),
            (_SCANLIMIT, 7),
            (_DECODEMODE, 0),
            (_SHUTDOWN, 1),
        ):
            self._write(command, data)

    def brightness(self, value):
        if not 0 <= value <= 15:
            raise ValueError("Brightness out of range")
        self._write(_INTENSITY, value)

    def text(self, s, x=0, y=0, c=1):
        super().text(s, x, y, c)

    def text_from_glyph(self, s, glyphs, x_offset=0, y_offset=0):
        col = 0
        for char in s:
            glyph = glyphs.get(char)

            if glyph:
                for y in range(8):
                    for x in range(8):
                        self.pixel(x+col+x_offset, y+y_offset, glyph[y][x])
            else:
                self.text(char, col+x_offset, y_offset)

            col += 8

    def show(self):
        for y in range(8):
            self._cs(0)
            for m in range(self._num):
                self._spi.write(bytearray([_DIGIT0 + y, self._buffer[(y * self._num) + m]]))
            self._cs(1)

    def zero(self):
        self.fill(0)

    def shutdown(self):
        self._write(_SHUTDOWN, 0)

    def wake(self):
        self._write(_SHUTDOWN, 1)

    def test(self, enable=True):
        self._write(_DISPLAYTEST, int(enable))


    def scroll_text(self, s, ms_delay=200):
        # self._buffer = bytearray(8 * self._num)
        # super().__init__(self._buffer, 8 * self._num, 8, MONO_HLSB)
        s_width = len(s) * 8
        n_pixels = self._num * 8

        self.zero()
        self.text_from_glyph(s, GLYPHS)
        for x in range(n_pixels, -s_width, -1):
            self.scroll(-1, 0)
            self.show()
            sleep_ms(ms_delay)
        return s_width

    def show_text(self, s):
        self.zero()
        self.text_from_glyph(s, GLYPHS)
        self.show()
