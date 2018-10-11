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
6. hit.c
  - クリック、あるいはタッチの当たり判定
  - ワールド座標からローカル座標への変換
7. label.c
  - ラベルによる文字の表示
8. layer-spotlight.c
  - レイヤーには合成方法を指定できる
  - これによって発光や反転を表現できる
  - このサンプルでは、スポットライト状の効果を作る
9. layer-particle.c
  - レイヤーには合成方法を指定できる
  - このサンプルでは、発光を表現する
10. layer-move.c
  - レイヤーの座標変更は、計算コストが小さい
  - ノードを大量に並べていても、高速に移動させることができる
11. action.c
  - アクションのサンプル


