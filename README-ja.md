# C言語による bi-core ライブラリと bi-ext ライブラリのサンプルプログラム

- https://github.com/bismite/bi-core
- https://github.com/bismite/bi-ext

1. texture.c
    - テクスチャを表示するだけ
2. texture-atlas.c
    - 一枚の画像から複数のテクスチャを得る
3. timer.c
    - タイマーを使ってテクスチャを回転させる
4. event-keyboard.c
    - キーボード入力の取得
5. event-touch.c
    - マウスあるいはタッチの取得
6. event-textinput.c
    - テキスト入力の取得
7. hit.c
    - クリック、あるいはタッチの当たり判定
    - ワールド座標からローカル座標への変換
8. label.c
    - ラベルによる文字の表示
9. layer-spotlight.c
    - レイヤーには合成方法を指定できる
    - これによって発光や反転を表現できる
    - このサンプルでは、スポットライト状の効果を作る
10. layer-particle.c
    - レイヤーには合成方法を指定できる
    - このサンプルでは、発光を表現する
11. layer-move.c
    - レイヤーの座標変更は、計算コストが小さい
    - ノードを大量に並べていても、高速に移動させることができる
12. action.c
    - アクションのサンプル

## macos

`./scripts/build.macos.sh` でビルドできる。

ビルド中に以下の二つのサブスクリプトが実行される。
`scripts/macos/install_sdl.sh` によって, SDL2, SDL2-image, SDL2-mixer のフレームワークが `~/Library/Frameworks` 以下にインストールされる。
`scripts/macos/install_glew.sh` によって、 `build/macos` ディレクトリで GLEW がコンパイルされる。

## linux

`./scripts/build.linux.sh` でビルドできる。

ビルドには clang, SDL2, SDL2-image, SDL2-mixer, GLEW が必要。
Ubuntuでは `sudo apt install clang libsdl2-dev libsdl2-mixer-dev libsdl2-image-dev libglew-dev` としてインストールできる。
