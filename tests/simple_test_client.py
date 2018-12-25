import socket
import argparse

TCP_IP = '127.0.0.1'  # localhost
BUFFER_SIZE = 1024


def send_n_assert(sock: socket.socket, mesg: str, excpected_result: str):
    sock.send(str.encode(mesg, 'utf-8'))
    response = sock.recv(BUFFER_SIZE).decode('utf-8')

    print("> " + mesg, end='')
    for line in response.split("\n"):
        if len(line) > 0:
            print("< " + line)

    assert response == excpected_result


def tests(sock: socket.socket):
    send_n_assert(sock, "INSERT A 0 zero\n", "OK\n")
    send_n_assert(sock, "INSERT A 0 lol\n", "ERR duplicate 0\n")
    send_n_assert(sock, "INSERT A 1 one\n", "OK\n")
    send_n_assert(sock, "INSERT A 2 two\n", "OK\n")
    send_n_assert(sock, "INSERT A 3 three\n", "OK\n")
    send_n_assert(sock, "INSERT A 4 four\n", "OK\n")
    send_n_assert(sock, "INSERT A 5 five\n", "OK\n")

    send_n_assert(sock, "INSERT B 3 cat\n", "OK\n")
    send_n_assert(sock, "INSERT B 4 dog\n", "OK\n")
    send_n_assert(sock, "INSERT B 5 kernel\n", "OK\n")
    send_n_assert(sock, "INSERT B 6 selector\n", "OK\n")
    send_n_assert(sock, "INSERT B 7 chanel\n", "OK\n")
    send_n_assert(sock, "INSERT B 8 kuzma\n", "OK\n")

    # test
    expected_intersect = "3,three,cat\n4,four,dog\n5,five,kernel\nOK\n"
    send_n_assert(sock, "INTERSECTION\n", expected_intersect)

    expected_sym_diff = "0,zero,\n1,one,\n2,two,\n6,,selector\n7,,chanel\n8,,kuzma\nOK\n"
    send_n_assert(sock, "SYMMETRIC_DIFFERENCE\n", expected_sym_diff)


def main():
    parser = argparse.ArgumentParser(description='Test Join Server')
    parser.add_argument('--port', action='store', dest='port', help='Port number')
    args = parser.parse_args()

    # Create a socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((TCP_IP, int(args.port)))
    tests(sock)
    sock.close()


if __name__ == '__main__':
    main()
