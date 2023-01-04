description = "Query domain features"

def setup_parser(subparser):
    subparser.add_argument('domain', nargs='+',
                            help='name(s) of domain(s) to query')

def query(parser, args):
    from ctypes import create_unicode_buffer, addressof, c_char_p

    from pyVariorum import variorum

    v = variorum.variorum()
    jsonbuf = create_unicode_buffer(1000)
    bufptr = c_char_p(addressof(jsonbuf))
    v.variorum_get_node_power_domain_info_json(bufptr)
    print(bufptr.value.decode("utf-8"))
