import sys

description = "get help on spack and its commands"
section = "help"
level = "short"

def setup_parser(subparser):
    help_cmd_group = subparser.add_mutually_exclusive_group()
    help_cmd_group.add_argument('help_command', nargs='?', default=None,
                                help='command to get help on')

    help_all_group = subparser.add_mutually_exclusive_group()
    help_all_group.add_argument(
        '-a', '--all', action='store_const', const='long', default='short',
        help='list all available commands and options')


def help(parser, args):
    if args.guide:
        print(guides[args.guide])
        return 0

    if args.help_command:
        parser.add_command(args.help_command)
        parser.parse_args([args.help_command, '-h'])
    else:
        sys.stdout.write(parser.format_help(level=args.all))
