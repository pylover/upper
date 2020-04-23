# upper

Upper standard input and forward to tcp, unix or tty device.

Suitable for controlling CNC and 3DPritners.


### Install

```bash
make
sudo make install
```

### Quickstart

```bash
upper /dev/ttyACM0  # Default baudrate: 115200
```

#### Help

```bash
upper --help
Usage: upper [OPTION...] OUTPUT
Serial Interface multiplexer

  -b, --baudrate=BAUDRATE    Baudrate, default: 115200
  -p, --port=TCPPORT         Remote TCP port, default: 5600
  -v, --verbose              Verbose Mode
  -?, --help                 Give this help list
      --usage                Give a short usage message
  -V, --version              Print program version

Mandatory or optional arguments to long options are also mandatory or optional
for any corresponding short options.

Report bugs to http://github.com/pylover/upper.
```

