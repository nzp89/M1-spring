**一旦お休みします。**

**aeif.c**: adaptive exponential integrate-and-fire neurons[1]のモデルを実装している。まだ途中。

**aeif_bistable.h**: aeif.cのパラメータセット。[1]参照。

**aeif_1neuron.c**: adaptive exponential integrate-and-fire neuron[2]のモデルを実装している。ルンゲクッタが終わらない。

**aeif_1neuron.h**: aeif_1neuron.cのパラメータセット。[2]参照。

**aeif_demo.c**: aeif_1neuron.cのデバッグをしてる時に生まれた。vのみを計算する。ルンゲクッタができた。

使い方

1. output_txtというフォルダを作る。(データを掃き出す先として。なくてもいいのか？)
2. make　(コンパイルしたい方の名前)
3. make run_(走らせたい方の名前)

参考文献

[1] https://www.frontiersin.org/articles/10.3389/fncom.2019.00019

[2] https://link.springer.com/article/10.1007/s00422-008-0264-7
