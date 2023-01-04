description = "List available features"

def setup_parser(subparser):
    subparser.add_argument(
        "-a",
        "--available",
        action="store_true",
        default=True,
        dest="available",
        help="list available features",
    )

def avail(parser, args):
    if args.available:
        print("avail")
