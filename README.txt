Image Processor

This tool is used for processing the BMP (24-bit depth w/ BITMAPINFOHEADER) image using filters.

It contains two executables:

1) ./image_processor - processes the picture.

Usage:
    [input_file_path] [output_file_path] [-filter] {parameters}
    input_file_path: Path to the file to be processed,
    output_file_path: Path to the processed result.
    To get the filters' options, type "image_processor [-filter_name] ".
    Available filter flags:
        1) -crop
        2) -sharp
        3) -edge
        4) -neg
        5) -gs
        6) -gs-basic
        7) -blur
        8) -curves

More info on "./image_processor" or "./image_processor -h.

2) ./tests - runs tests ensuring the expected working process. Use "./tests"



!!!VIDEO!!!
- https://disk.yandex.ru/i/9n3aVis_IOU2Qw
- https://disk.yandex.ru/i/ljcDrx6fQf0w0w