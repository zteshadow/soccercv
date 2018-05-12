
import socket

def check_port(address, port):
    s = socket.socket()
    try:
        s.settimeout(1.0) #set timeout
        s.connect((address, port))
        print(str(address), ': YES')
        return True
    except:
        print(str(address), ': NO')
        return False

#for example, self IP is 172.20.10.3, then mask is 172.20.10, start is 1, end is 255
def check_server(mask, start, end):
    ip = start
    while ip <= end:
        address = str(mask) + '.' + str(ip)
        ip += 1
        check_port(address, 80)

if __name__=='__main__':
    check_server('172.20.10', 1, 255)
