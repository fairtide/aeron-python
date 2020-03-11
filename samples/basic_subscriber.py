import sys
from argparse import ArgumentParser, ArgumentError

import aeronpy
import numpy as np

messages = 0
count = 0

data_log = np.zeros(int(1e6) + 1, dtype=int)


def main():
    try:
        parser = ArgumentParser()
        parser.add_argument('-p', '--prefix', default=None)
        parser.add_argument('-c', '--channel', default='aeron:udp?endpoint=localhost:40123')
        parser.add_argument('-s', '--stream_id', type=int, default=1)

        args = parser.parse_args()
        context = aeronpy.Context(
            aeron_dir=args.prefix,
            new_subscription_handler=lambda *args: print(f'new subscription {args}'),
            available_image_handler=lambda *args: print(f'available image {args}'),
            unavailable_image_handler=lambda *args: print(f'unavailable image {args}'))

        def process_data(data_bytes):
            global count, messages
            messages += 1

            data = np.frombuffer(data_bytes, dtype=float)
            nbytes = len(bytes(data_bytes))
            count += nbytes

            data_log[int(data[0])] += 1

            if messages % 100000 == 0:
                print("Messages %d total bytes %d" % (messages, count))

        subscription = context.add_subscription(args.channel, args.stream_id)
        while True:
            fragments_read = subscription.poll(process_data)

            if fragments_read == 0:
                eos_count = subscription.poll_eos(lambda *args: print(f'end of stream: {args}'))
                if eos_count > 0:
                    break

        print("Messages %d total bytes %d, bytes per message %f" % (messages, count, count / messages))
        print("Printing the ids of the messages with a received count !=1"
              ", the index should be starting at number of messages")
        print(np.where(data_log != 1)[0][:5])

        return 0

    except ArgumentError as e:
        print(e, file=sys.stderr)
        return -1
    except Exception as e:
        print(e, file=sys.stderr)
        return -2


if __name__ == '__main__':
    # input("Press Enter to continue...")

    main_return_code = main()

    exit(main_return_code)
