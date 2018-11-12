import sys
from argparse import ArgumentParser, ArgumentError
from datetime import timedelta
from time import sleep

from aeronpy import Context, BACK_PRESSURED, NOT_CONNECTED, ADMIN_ACTION, PUBLICATION_CLOSED


def main():
    try:
        parser = ArgumentParser()
        parser.add_argument('-p', '--prefix', default=None)
        parser.add_argument('-c', '--channel', default='aeron:udp?endpoint=localhost:40123')
        parser.add_argument('-s', '--stream_id', type=int, default=1)
        parser.add_argument('-m', '--messages', type=int, default=sys.maxsize)
        parser.add_argument('-l', '--linger', type=int, default=60*60*1000)

        args = parser.parse_args()
        context = Context(
            aeron_dir=args.prefix,
            resource_linger_timeout=timedelta(milliseconds=args.linger),
            new_publication_handler=lambda *args: print(f'new publication - {args}'))

        publication = context.add_publication(args.channel, args.stream_id)
        for i in range(args.messages):
            result = publication.offer(f'Hallo World! {i}')
            if result == BACK_PRESSURED:
                print('Offer failed due to back pressure')
            elif result == NOT_CONNECTED:
                print('Offer failed because publisher is not connected to subscriber')
            elif result == ADMIN_ACTION:
                print('Offer failed because of an administration action in the system')
            elif result == PUBLICATION_CLOSED:
                print('Offer failed publication is closed')
            else:
                print('yay!')

            sleep(1)

        return 0

    except ArgumentError as e:
        print(e, file=sys.stderr)
        return -1
    except Exception as e:
        print(e, file=sys.stderr)
        return -2


if __name__ == '__main__':
    exit(main())
