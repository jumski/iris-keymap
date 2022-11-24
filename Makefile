.PHONY: main compile flash

main: compile flash

compile: 
	qmk compile --keyboard keebio/iris/rev6b --keymap jumski

flash:
	qmk flash /home/jumski/qmk_firmware/.build/keebio_iris_rev6b_jumski.hex
