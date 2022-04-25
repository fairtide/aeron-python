import sys
from argparse import ArgumentParser, ArgumentError
from time import sleep, time

from aeronpy import Context

running = False

def handler(data, header):
    global running
    if header.flags & 0x40:
        running = False
        print("end")
    if header.flags & 0x80:
        print("start")
        running = True

def main():
    try:
        parser = ArgumentParser()
        parser.add_argument('-p', '--prefix', default=None)
        parser.add_argument('-c', '--channel', default='aeron:udp?endpoint=localhost:40123')
        parser.add_argument('-s', '--stream_id', type=int, default=1)

        args = parser.parse_args()
        context = Context(
            aeron_dir=args.prefix,
            new_subscription_handler=lambda *args: print(f'new subscription {args}'),
            available_image_handler=lambda *args: print(f'available image {args}'),
            unavailable_image_handler=lambda *args: print(f'unavailable image {args}'))

        subscription = context.add_subscription(args.channel, args.stream_id)
        while True:
            fragments_read = subscription.poll(handler)
            if fragments_read != 0:
                print(f"running: {running}")
                start = time()
                while running:
                    fragments_read = subscription.poll(handler)
                print("elapsed: ", time() - start, " running: ", running)
            #if fragments_read == 0:
            #    eos_count = subscription.poll_eos(lambda *args: print(f'end of stream: {args}'))
            #    if eos_count > 0:
            #        break
            sleep(0.001)

        return 0

    except ArgumentError as e:
        print(e, file=sys.stderr)
        return -1
    except Exception as e:
        print(e, file=sys.stderr)
        return -2


if __name__ == '__main__':
    exit(main())
