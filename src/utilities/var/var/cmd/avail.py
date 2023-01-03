description = "List available features"

def setup_parser(subparser):
    subparser.add_argument('features', nargs='?', default=None,
                            help='list available features')

def avail(args):
    print("avail")
