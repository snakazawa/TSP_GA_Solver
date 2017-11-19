# TSP GA Solver

GAを用いたTSP(Traveling Salesman Problem)のSolverです。

## Requirements

- C++ (>=11)

## Run

```
make

./main_er_init1 < samples/sample6.txt
```

## Config

個体数や出力先、アルゴリズムはプリプロセッサ定義により指定できます。  
`constants.h` や `Makefile` を参照してください。

## Algorithm

### 個体の初期化

`INIT_TYPE` によりアルゴリズムを設定します。  

- "rand" : ランダムに生成する (デフォルト)
- "src" : 他の結果を使う (エリート戦略により決定, 重複削除)

### 交叉

`CROSSOVER_TYPE` によりアルゴリズムを選択します。

- "ox": Ordered crossover
- "er": Edge recombination crossover (デフォルト)

ERにおいて、次の頂点が選択できないときは、未到達の頂点のうち現在の頂点から近い頂点を最大5個選び、その中からランダムに選択します。  
ERはOXよりも速度は遅くなりますが性能は良いです。

### 突然変異

次のアルゴリズムの中からランダムに1つ決定し適用します。

- Two Point Reverse
- Two Point Change

突然変異確率は `P_MUTATION` で指定できます (デフォルト=0.01=1%)

### 選択

エリート戦略により選択します。

## 参考資料

- [ono\-t\.d\.dooo\.jp/GA/GA\-order\.html](http://ono-t.d.dooo.jp/GA/GA-order.html)
- [巡回セールスマン問題と遺伝的アルゴリズム\(GA\)](https://www.studiok-i.net/tsp/)
- [遺伝的アルゴリズム（Genetic Algorithm）を始めよう！](https://www.slideshare.net/kzokm/genetic-algorithm-41617242)
