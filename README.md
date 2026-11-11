<img width="1419" height="1121" alt="IMG_9147" src="https://github.com/user-attachments/assets/be68df35-f92b-4f08-bf51-6f54029fb442" />
Arduinoだと、すぐにこれが出来てしまうけど、あえて ラズパイPicoでやる場合は
命令文でvoid setup()の場所に
setSDA(Pin番号)
setSCL(Pin番号)
Pin番号はPico Pinoutで検索して出てくるピン配置でSDA、SCLとなっていて、しかもTx、Rxとなっているピン端子でないといけない
