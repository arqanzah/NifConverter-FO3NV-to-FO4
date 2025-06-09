Dim x, y, xx, yy, result4x, result4y, result8x, result8y
const dumper = 32768, LodLevel4 = 4, LodLevel8 = 8, LodLevel16 = 16
x = InputBox ("Warning! Only for Wasteland worldspace." + chr(13) + "Input your cell coordinate X", "LOD file finder")
y = InputBox ("Warning! Only for Wasteland worldspace." + chr(13) + "Input your cell coordinate Y", "LOD file finder")
xx = CDbl(x)
yy = CDbl(y)
result4x = ((xx + dumper) - ((xx + dumper) mod LodLevel4)) - dumper
result4y = ((yy + dumper) - ((yy + dumper) mod LodLevel4)) - dumper
result8x = ((xx + dumper) - ((xx + dumper) mod LodLevel8)) - dumper
result8y = ((yy + dumper) - ((yy + dumper) mod LodLevel8)) - dumper
result16x = ((xx + dumper) - ((xx + dumper) mod LodLevel16)) - dumper
result16y = ((yy + dumper) - ((yy + dumper) mod LodLevel16)) - dumper
MsgBox ( "LOD files for your cell:" + chr(13) + "Level 4:      wasteland.level4.x" & result4x & ".y" & result4y & ".nif" + chr(13) + "Level 8:      wasteland.level8.x" & result8x & ".y" & result8y & ".nif" + chr(13) + "Level 16:      wasteland.level16.x" & result16x & ".y" & result16y & ".nif")
