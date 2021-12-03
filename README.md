# TMP102

Simple library for TMP102 temperature sensor produced by TI. I created option to work with float numbers (that is simple)
or with ints (that is simple but code looks like stupid).

Basic functions - Read16/Write16 are only ones that are using HAL, in case of application on different system those two have to be edited.
All the details you can find in the function descriptions or in a datasheet -> link below.

https://www.ti.com/lit/ds/symlink/tmp102.pdf?ts=1638440032427&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FTMP102%253Futm_source%253Dgoogle%2526utm_medium%253Dcpc%2526utm_campaign%253Dasc-null-null-GPN_EN-cpc-pf-google-wwe%2526utm_content%253DTMP102%2526ds_k%253DTMP102%2BDatasheet%2526DCM%253Dyes%2526gclid%253DEAIaIQobChMIwIPi3fDE9AIVBap3Ch03kAv-EAAYASAAEgIb-fD_BwE%2526gclsrc%253Daw.ds
