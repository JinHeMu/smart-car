import sensor
import image
import time
import seekfree, pyb



lcd = seekfree.LCD180(1)
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)

for i in range(5):
    img = sensor.snapshot()
    lcd.show_image(img, 160, 120, zoom=2)
    filename = "/picture/example%d.jpg" % i
    img.save(filename)
    time.sleep(1)
