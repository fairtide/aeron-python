import sys
from argparse import ArgumentParser, ArgumentError
from datetime import timedelta
from time import sleep

import numpy as np
from aeronpy import Context, BACK_PRESSURED, NOT_CONNECTED, ADMIN_ACTION, PUBLICATION_CLOSED


def main():
    try:
        parser = ArgumentParser()
        parser.add_argument('-p', '--prefix', default=None)
        parser.add_argument('-c', '--channel', default='aeron:udp?endpoint=localhost:40123')
        parser.add_argument('-s', '--stream_id', type=int, default=1)
        parser.add_argument('-m', '--messages', type=int, default=1e5)
        parser.add_argument('-ms', '--messagesize', type=int, default=250)
        parser.add_argument('-l', '--linger', type=int, default=60 * 60 * 1000)

        args = parser.parse_args()

        context = Context(
            aeron_dir=args.prefix,
            resource_linger_timeout=timedelta(milliseconds=args.linger),
            new_publication_handler=lambda *args: print(f'new publication - {args}'))
        import time
        start = time.time()
        publication = context.add_publication(args.channel, args.stream_id)
        i = 0
        while i < args.messages:
            data = np.random.uniform(size=args.messagesize)
            data[0] = i
            data_bytes = data.tobytes()

            while True:
                result = publication.offer(data_bytes)

                if result >= 0:
                    i += 1
                    break
                else:
                    if result == BACK_PRESSURED:
                        print('Offer failed due to back pressure')
                    elif result == NOT_CONNECTED:
                        print('Offer failed because publisher is not connected to subscriber')
                    elif result == ADMIN_ACTION:
                        print('Offer failed because of an administration action in the system')
                    elif result == PUBLICATION_CLOSED:
                        print('Offer failed publication is closed')

                    sleep(.5)
        end = time.time()
        print("%d,%d,%.3f" % (args.messages, args.messagesize, end - start))
        # with open("log.csv", "at") as f:
        #     f.write("%d,%d,%.3f\n" % (args.messages, args.messagesize, end - start))

        return 0

    except ArgumentError as e:
        print(e, file=sys.stderr)
        return -1
    except Exception as e:
        print(e, file=sys.stderr)
        return -2


if __name__ == '__main__':
    ret = main()
    exit(ret)
