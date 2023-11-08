
#@#= マッチメイキング機能について
= 同期処理？
#@#この章では、『PlayersLink』でどのようにマッチメイキングを実現しているかについて話していきます。
この章では、同期処理について説明していきます。



== Netcode for GameObjects
Relayサーバーを経由してホストとクライアントの接続が完了するとNetcode for GameObjects(以下、NGO)という
ライブラリを使って同期処理を行うことができます。
NGOは、通常のMonoBehaviourかNetworkBehaviourに切り替えて使用します。
NetworkBehaviourは、MonoBehaviourを継承して主にTransformクラスの情報をホスト(サーバー)にRPCで送ることで
位置情報の同期を図ります。



=== RPCについて

==== ServerRPC

RequireOwnership

==== ClientRPC


==== Network Transform
==== Network Animation
Network Variable 



RPCは『Remote Procedure Call』の略です。平たく言えば、他の端末のメソッドを実行できる機能です。
下記の様に[ServerRPC]または[ClientRPC]のアトリビュートを付けることでRPCの対象メソッドとして指定することができます。

PhotonでRPCを使用したことがある方ならピンとくるところがあるかもしれません。
しかし、Photonで使える[RPC]アトリビュートとは勝手が違うので注意が必要です。

ServerRPCとClientRPCは、ホスト(サーバー)とクライアントで呼び出し権限が異なるからです。
下図をご覧ください。

//indepimage[5_1_1_RPCについて][Ready機能]{
  2023年11月時点で一般的なゲームのロビー画面
//}

これは各RPCの呼び出し権限を表した表です。
UGSから提供されているゲームサーバーホスティング機能を使うとホストとサーバーを分離できるため、表にはサーバーも登場人物として記載されていますが、
PlayersLinkでは、今回の例ではゲームサーバーホスティング機能は使っていないため、見るべきはホストとクライアントです。
ホストは、サーバーでありクライアントでもあるため、ServerRPC送信/実行とClientRPC送信/実行のすべてに対して権限があります。
一方でクライアントは、ServerRPCの実行とClientRPCの送信の権限がありません。
そのため、



基本的にServerRPCでホスト側のメソッドを実行し、ホスト側でClientRPCを起動して全体に
