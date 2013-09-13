import time
import grapevine

UPDATE_FREQUENCY = 10

if __name__ == '__main__':
    com = grapevine.Communicator('util/communication/mock_bar', buffer_len=0)

    while True:
        #print com.get_last_message('util/communication/mock_foo')
        print com.get_message_buffer('util/communication/mock_foo')
        time.sleep(UPDATE_FREQUENCY)

