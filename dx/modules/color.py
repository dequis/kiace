
def main():
    import gtk
    dialog = gtk.ColorSelectionDialog("dx")
    dialog.move(30, 30)
    dialog.run()
    print dialog.colorsel.get_current_color()
