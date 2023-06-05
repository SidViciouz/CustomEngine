start fxc.exe -E "VS" -Fo DefaultVS.cso -T vs_5_1 default.hlsl
start fxc.exe -E "PS" -Fo DefaultPS.cso -T ps_5_1 default.hlsl

start fxc.exe -E "VS" -Fo LineVS.cso -T vs_5_1 Line.hlsl
start fxc.exe -E "PS" -Fo LinePS.cso -T ps_5_1 Line.hlsl

start fxc.exe -E "VS" -Fo PBRVS.cso -T vs_5_1 PBR.hlsl
start fxc.exe -E "PS" -Fo PBRPS.cso -T ps_5_1 PBR.hlsl