import comm

sub = Arduino("/dev/ttyACM0")
sub.go()

try: 
    sub.wait_for_armed(True)
finally:
    sub.stop()




