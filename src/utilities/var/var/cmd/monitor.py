description = "Sample performance counters"

def setup_parser(subparser):
    subparser.add_argument(
        "-a",
        "--application",
        required=True,
        dest="application",
        help="application and arguments",
    )

def monitor(parser, args):
    print("monitoring", args.application)
