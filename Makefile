.PHONY: main compile flash download_latest flash_latest

main: compile flash

compile: 
	qmk compile --keyboard keebio/iris/rev6b --keymap jumski

flash:
	qmk flash /home/jumski/qmk_firmware/.build/keebio_iris_rev6b_jumski.hex

flash_latest: download_latest flash

download_latest:
	gh release download --pattern '*.hex' --clobber
	mv keebio_iris_rev6b_jumski.hex /home/jumski/qmk_firmware/.build/

