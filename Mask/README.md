# Mask

My teammate (Snowflake) who started working on this challenge started 
implementing a QR-Code reader to decode the partial qr code. This code
can be seen in `mask_attack.py`. I later added code to calculate the actual
expected error correction codes for any string we wanted.

Then I re-implemented the whole error correction code using z3 and with only two
partial qr codes this was already solveable in about 8 seconds and after around
two minutes it spat out another potential solution `actf{not_quadrhc}` along
with the message that there are no more solution than this and `actf{not_quadres}`.

