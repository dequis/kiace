#!/usr/bin/python

import sys

# you might want to use this, but setting PYTHONPATH is preferred
#sys.path.append("/home/dx/lib")

import dx_modules

class DxError(Exception):
    pass

def main():
    # branching? what is that?
    if len(sys.argv) > 1:
        try:
            module = __import__('dx_modules.' + sys.argv[1], fromlist=[sys.argv[1]])
        except ImportError:
            raise DxError("Module not found")
        else:
            if len(sys.argv) > 2:
                if hasattr(module, sys.argv[2]):
                    getattr(module, sys.argv[2])()
                else:
                    raise DxError("Method not found in module")
            else:
                getattr(module, 'main')()
    else:
        print ', '.join(dx_modules.list())


if __name__ == '__main__':
    try:
        main()
    except DxError, e:
        print "Error:", e
