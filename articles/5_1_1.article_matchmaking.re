
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

==== ServerRpc

RequireOwnership

==== ClientRpc


==== Network Transform
==== Network Animation
Network Variable 



RPCは『Remote Procedure Call』の略です。平たく言えば、他の端末のメソッドを実行できる機能です。
下記の様に[ServerRpc]または[ClientRpc]のアトリビュートを付けることでRPCの対象メソッドとして指定することができます。

//emlist[][C]{

  [ServerRpc]
  private void TestServerRpc()
  {
    Debug.Log($"NoticeJoinedClientRpc");
  }

  [ClientRpc]
  private void NoticeJoinedClientRpc()
  {
    Debug.Log($"NoticeJoinedClientRpc");
  }
}


PhotonでRPCを使用したことがある方ならピンとくるところがあるかもしれません。
しかし、Photonで使える[PunRPC]アトリビュートとは勝手が違うので注意が必要です。

というのも、ServerRpcとClientRpcはホスト(サーバー)とクライアントで呼び出し権限が異なるからです。
下図をご覧ください。

//indepimage[5_1_1_RPCについて][各RPCの権限について]{
//}

これは各RPCの呼び出し権限を表した表です。
UGSから提供されているゲームサーバーホスティング機能を使うとホストとサーバーを分離できるため、表にはサーバーも登場人物として記載されていますが、
今回の例ではゲームサーバーホスティング機能は使っていないため、見るべきはホストとクライアントです。
ホストは、サーバーでありクライアントでもあるため、ServerRpc送信/実行とClientRpc送信/実行のすべてに対して権限があります。
一方でクライアントは、ServerRpcの実行とClientRpcの送信の権限がありません。
ホストA、クライアントB、クライアントCがいる状況で、クライアントBからクライアントCにならんかの処理を実行させたい場合には、ホストAのServerRpcメソッドを呼び出して
そこからClientRPCを呼び出す様な処理が必要になります。

今回は『Multiplayer Samples Utilities』というライブラリを使用しています。2022年にUGSを使ったマルチプレイヤーゲーム開発のデモとして
『Boss Room」というプロジェクトがUnityから公開されており、そこで使用されているユーティリティライブラリです。
NGOではオブジェクトのTransform情報を同期するためにNetworkTransformというコンポーネントが元から用意されているのですが、
これをクライアント用に特化して作られた