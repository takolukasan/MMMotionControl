# MMMotionControl

https://github.com/takolukasan/MMMotionControl  
DXOpenNIの仕組みを利用し、MMDモーションキャプチャ機能にボーン位置を流し込むDLL  
You can VirBiNiku with MMD motion capture function. Uses DXOpenNI scheme.

現状頭位置情報のみ、MikuMikuVRと連携し入力する想定で設計してあります。  
Now, only head position tracking. It was designed to work with MikuMikuVR.

ソースコード内にモデルのボーン初期位置値をハードコートする作りになっています。  
各自で使用するモデルのボーン位置に書き換えるか、外部から動的に入力できるよう改造してください。  
The model each bone positions are wrriten in source code (constant variables).  
You can modify those values, or modify it so those values input from outside dynamically.  

ビルド環境 / Build environments  
* Visual Studio 2008 Std  
* Microsoft DirectX SDK June 2010

利用条件等は https://github.com/takolukasan/MikuMikuVR に準じます(README.md参照)  
License conditions following: https://github.com/takolukasan/MikuMikuVR (See README.md)  
