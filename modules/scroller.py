from max7219 import Matrix8x8
from time import sleep_ms

class Scroller(Matrix8x8):
    def scroll_text(self, s, ms_delay=20):
        s_width = len(s) * 8
        n_pixels = self._num * 8
        for x in range(n_pixels, -s_width, -1):
            self.zero()
            self.text(s, x)
            self.show()
            sleep_ms(ms_delay)
            self.show()
        return s_width
    
    def display_text(self, s):
        self.text(s)
        self.show()
