import ossaudiodev
import threading
import Queue

import gobject
import gtk

import plotdev

FPS = 5

class CairoPlot(plotdev.PlotDev):
    def __init__(self, *args):
        plotdev.PlotDev.__init__(self, *args)
        self.thread = GtkThread()
        self.thread.start()

    def _plot(self, samples):
        ff = 1 << self.PARSERS[self._format][1]
        for sample in self._parse(samples):
            self.thread.feed(float(sample) / ff)

    def close(self):
        plotdev.PlotDev.close(self)
        gobject.idle_add(gtk.main_quit)
        self.thread.join()

class PlotScreen(gtk.DrawingArea):
    __gsignals__ = {'expose-event': 'override'}

    def __init__(self):
        gtk.DrawingArea.__init__(self)
        self.input = Queue.Queue()
        self.alive = True

    def do_expose_event(self, event):
        cr = self.window.cairo_create()
        cr.rectangle(event.area.x, event.area.y, event.area.width, event.area.height)
        cr.clip()

        width, height = self.window.get_size()

        cr.set_source_rgb(0.0, 0.0, 0.0)
        cr.rectangle(0, 0, width, height)
        cr.fill()

        if not self.input.empty():
            cr.set_source_rgb(0.0, 1.0, 0.0)
            cr.set_line_width(1.0)
            try:
                ly = 0
                for x in xrange(width):
                    y = self.input.get_nowait()
                    if y != ly:
                        cr.line_to(x, y * height / 3 + height / 2)
                        ly = y
            except Queue.Empty:
                pass
            cr.stroke()


class GtkThread(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
        gobject.threads_init()
        gtk.gdk.threads_init()
        self.screen = PlotScreen()
        
        def callback():
            self.screen.queue_draw()
            return True
        self.timeout_id = gobject.timeout_add(1000 / FPS, callback)

    def feed(self, data):
        if not self.screen.alive:
            raise SystemExit
        self.screen.input.put(data)

    def run(self):
        win = gtk.Window()
        win.connect('delete-event', gtk.main_quit)
        win.add(self.screen)
        win.show_all()
        gtk.main()
        self.screen.alive = False
        

def open(*args):
    return CairoPlot(ossaudiodev.open(*args))
