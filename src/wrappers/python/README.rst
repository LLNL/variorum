# PyVariorum

PyVariorum is a python module for exposing Variorum's C API for power and
performance management with the help of `ctypes`. Variorum users can now
utilize pyVariorum to obtain power, thermal and other information from their
python applications. All print, cap, JSON, topology, and miscellaneous APIs are
supported in pyVariorum, with the exception of the poll and monitoring APIs.

Once the Variorum library has been built and installed on the underlying
architecture, and can be found with `LD_LIBRARY_PATH` for example, PyVariorum
can be easily installed with pip as follows:

```
    $ cd src/wrappers/python
    $ pip3 install .
```

You may also need to update your PYTHONPATH with the path to the python wrappers directory:

```
    $ cd src/wrappers/python
    $ export PYTHONPATH=$PWD:$PYTHONPATH
```

Please refer to `src/examples/python-examples` to see usage of pyVariorum.
