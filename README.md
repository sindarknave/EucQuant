# EucQuant

This houses edoQuant, a quantizer module for VCV rack to generate fun xenharmonic melodies from control voltage.

- *Input* and *Output* receive and send 1V/oct pitch information.
- *edo* (equal division of the octave) represents a tuning obtained by dividing the octave into n steps. Takes the values 1-24.
- *Restrict* further quantizes voltage into Euclidean periods between the equal divisions. For instance the parameter (11, 4) would quantize voltages into the (0, 4/11, 8/11) divisions of the octave.
- *Offset* adds a voltage offset before the input is quantized.

Licensed with MIT.