# TSP GA Solver

GAを用いたTSP(Traveling Salesman Problem)のSolverです。

## Requirements

- C++ (>=11)

## Hot to use

200頂点のsampleの算出例です。

```
make

./main_ox_init1 < samples/sample6.txt
./main_er_init1 < samples/sample6.txt

# `results_init1/` にある程度ファイルが溜まったら下を実行する
./main_ox_init2 < samples/sample6.txt

# `results_init2/` にある程度ファイルが溜まったら下を実行する
./main_ox_init3 < samples/sample6.txt

# 最高スコアを表示する
cat results_init3`ls results_init3 | head -n1`
```

## Config

個体数や出力先、アルゴリズムはプリプロセッサ定義により指定できます。  
`constants.h` や `Makefile` を参照してください。

## Algorithm

### 個体の初期化

`INIT_TYPE` によりアルゴリズムを設定します。  

- "rand" : ランダムに生成する (デフォルト)
- "src" : 他の結果を使う (ランキング方式により決定)

### 交叉

`CROSSOVER_TYPE` によりアルゴリズムを選択します。

- "ox": Ordered crossover
- "er": Edge recombination crossover

ERにおいて、次の頂点が選択できないときは、未到達の頂点のうち現在の頂点から近い頂点を最大5個選び、その中からランダムに選択します。  
ERはOXよりも10倍ほど遅くなります。

### 突然変異

次のアルゴリズムの中からランダムに1つ決定し適用します。

- Two Point Reverse
- Two Point Change
- 2点間の経路をシャッフル

突然変異確率は `P_MUTATION` で指定できます (デフォルト=1.0=100%)

### 選択

エリート戦略により選択します。

## 参考資料

- [ono\-t\.d\.dooo\.jp/GA/GA\-order\.html](http://ono-t.d.dooo.jp/GA/GA-order.html)
- [巡回セールスマン問題と遺伝的アルゴリズム\(GA\)](https://www.studiok-i.net/tsp/)
- [遺伝的アルゴリズム（Genetic Algorithm）を始めよう！](https://www.slideshare.net/kzokm/genetic-algorithm-41617242)
