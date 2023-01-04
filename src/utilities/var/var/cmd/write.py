description = "Write a value to a register"

def setup_parser(subparser):
    subparser.add_argument('regno', nargs=1,
                            help='register value')
    subparser.add_argument('value', nargs=1,
                            help='value to write')
    subparser.add_argument('-a', "--all",
        action="store_true",
        default=False,
        dest="all",
        help="write to all processors (default False)"
    )
    subparser.add_argument('-p', "--processor",
        default=0,
        type=int,
        dest="proc",
        help="select processor number (default 0)"
    )

def write(parser, args):
    print("write value regno=", args.regno, "value=", args.value, "allprocs=",
args.all, "cpuid=", args.proc)
