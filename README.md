# 猫の記憶  
制作時期：2022年7月~2023年5月  
制作人数：1人  
GitHub：https://github.com/daiki2001/Job-Hunting-Work.git  

## 作品概要
主人公である猫を操作してゲームを進めていく3Dパズルゲームです。  
### 操作説明
* 移動：WASDキー
* 選択：十字キー
* 決定：スペースキー
* タイトルに戻る：F1キー
* 強制終了：Escキー
#### 操作説明（ステージエディターのみ）
* ステージのセーブ：Ctrlキー \+ Sキー
* 操作を一つ戻す：Ctrlキー \+ Zキー
* 操作をやり直す：Ctrlキー \+ Yキー

## 開発環境
OS:Windos11  
DirectX 12  
Visual Studio 2022
### 使用しているライブラリ
* [DirectXTex](https://github.com/microsoft/DirectXTex)
* [FBXSDK](https://www.autodesk.com/products/fbx/overview)
* [nlohmanjson](https://github.com/nlohmann/json)

## 開発エピソード
* 就職作品のメイン作品を制作するにあたり、「今までやってきた事を結集させたい」という考えから、今まで制作に関わってきたほとんどのゲームでステージ周りのことをやっていたのもあり、早い段階でマップチップベースのステージにする事に決まった。
* ゲームと平行してステージエディターの制作もしたが、ゲーム部分を作る時と同じかそれ以上のモチベーションで開発を進めることが出来、結果としてゲーム部分よりも先にステージエディターが出来た。

## アピールポイント
* `Game - Stage.cpp MiniMap()`  
自機のいる周囲の部屋の有無と部屋同士の繋がり方が分かる物。
* `StageEditer - FuncRedoUndo.h, FuncRedoUndo.cpp`  
実体を生成することでどこでも「アンドゥ／リドゥ」の機能を利用することが出来るクラス。

## 脚注
### 外部素材
[猫](https://www.cgtrader.com/free-3d-models/animals/mammal/cat-8f299065-4ddc-4308-a0f8-510f08be3ee4)
