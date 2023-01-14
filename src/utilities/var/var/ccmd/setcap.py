description = "Set a frequency or power cap"

def setup_parser(subparser):
    subparser.add_argument('domain', nargs=1,
                            help='domain')
    subparser.add_argument('value', nargs=1,
                            help='cap value')

def setcap(parser, args):
    print("cap domain=", args.domain, "cap=", args.value)
