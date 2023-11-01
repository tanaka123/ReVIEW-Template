
= 事前準備

この章ではUGSを使用する上で必要な準備について書いていきます。
このあたりはネット上に情報が山ほどあるので簡単な説明とさせていただきます。

== プロジェクトの作成
これは言わずもがな。今回はUnity2022.3.10f1を使用します。

== UGSにログイン
https://dashboard.unity3d.com/gaming


== UGSとプロジェクトをリンク

== パッケージのインポート
下記がインポートされていることを確認してください。
*Lobby
*Relay
*UTP
*ParrelSync
*Netcode for GameObjects


== ParrelSyncについて
UnityEditorを複製してマルチプレイのテストを可能にしてくれるツールです。
いちいちビルドしなくてもこのツールを使えば、簡単に複数人でアクセスしたテストができます。

== WebGLでのビルドについて
RelayではWebScketを使用します。
Unityのシーン上でNetworkMangerのUse Web Scketを有効化する必要があります。
PlayersLinkではこの辺りの処理を自動化しています。

== Netcode for GameObjectsとは
Lobby,Relayを使用してプレイヤー同士を同期した後、
シーン上のプレイヤーの位置情報(Transform)やRPC処理を飛ばすの使用するライブラリです。
このあたりは通常のUnity開発をやったことがある方ならある程度とっつきやすいのではにかと思います。

