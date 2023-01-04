description = "Read value from register"

def setup_parser(subparser):
    subparser.add_argument('regno', nargs=1,
                            help='register value')
    subparser.add_argument('-x', "--hexadecimal",
        action="store_true",
        default=False,
        dest="hex",
        help="hexadecimal output (lower case)"
    )
    subparser.add_argument('-X', "--capital-hex",
        action="store_true",
        default=False,
        dest="caphex",
        help="hexadecimal output (upper case)"
    )
    subparser.add_argument('-d', "--decimal",
        action="store_true",
        default=False,
        dest="decimal",
        help="signed decimal output"
    )
    subparser.add_argument('-u', "--unsigned",
        action="store_true",
        default=False,
        dest="unsigned",
        help="unsigned decimal output"
    )
    subparser.add_argument('-o', "--octal",
        action="store_true",
        default=False,
        dest="octal",
        help="octal output"
    )
    subparser.add_argument('-c', "--c-language",
        action="store_true",
        default=False,
        dest="clang",
        help="C language constant output"
    )
    subparser.add_argument('-0', "--zero-pad",
        action="store_true",
        default=False,
        dest="zero",
        help="output leading zeroes"
    )
    subparser.add_argument('-r', "--raw",
        action="store_true",
        default=False,
        dest="raw",
        help="raw binary output"
    )

    subparser.add_argument('-a', "--all",
        action="store_true",
        default=False,
        dest="all",
        help="read from all processors (default False)"
    )
    subparser.add_argument('-p', "--processor",
        default=0,
        type=int,
        dest="proc",
        help="select processor number (default 0)"
    )
    subparser.add_argument('-f', "--bitfield",
        required=True,
        type=str,
        dest="bitfield",
        help="output bits [h:l] only"
    )

def read(parser, args):
    print("read value regno=", args.regno, "allprocs=", args.all, "cpuid=", args.proc)
