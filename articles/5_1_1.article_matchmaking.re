= 同期処理
この章では、Netcode for GameObjectsを使った同期処理について説明していきます。

== Netcode for GameObjectsについて
Relayサーバーを経由してホストとクライアントの接続が完了するとNetcode for GameObjects(以下、NGO)という
ライブラリを使って同期処理を行うことができます。
すでにRealyの章でも登場していたNetworkManagerもNGOのコンポーネントです。
NetworkManagerは、先に見た通りホスト(サーバー)とクライアントの接続を管理しています。
実際に同期させるオブジェクトには、NetworkObjectをアタッチします。

//indepimage[5_1_1_PlayerObject][NetworkObject]{
//}

PlayersLinkでは、同期させる人型オブジェクトにNetworkObjectをアタッチしています。
このプレイヤーとなるキャラクターのプレハブをNetworkManagerのPlayerPrefabにセットしています。

//indepimage[5_1_2_NetworkManager_PlayerPrefab][NetworkObject]{
//}

こうすることで、NetworkManagerのStartHost、StartClientが実行されると同時にキャラクタープレハブがシーン上にスポーンされます。
PlayersLinkでは、Unityで無料配布されている『Starter Assets - Third Person Charactor Controller』というアセットの
PlayerArmatureをキャラクタープレハブとして使用しています。
下図は、PlayersLinkでマッチング完了後に始まるメインゲームのサンプルです。

//indepimage[5_1_3_PlayersLinkのサンプルゲーム][NetworkObject]{
//}

建物の上にあるゴールを目指すシンプルなゲームです。


//indepimage[5_1_4_PlayersLinkのサンプルゲーム2][NetworkObject]{
//}

ゴール後、ロビーに変えるためのポップアップが表示されます。
ロビーに戻るときにNetworkManagerのホスト/クライアントの停止が行われることでRelayサーバーは削除されます。


== RPCについて

RPCは『Remote Procedure Call』の略です。平たく言えば、他の端末のメソッドを実行できる機能です。
NGOでは、下記の様に[ServerRpc]または[ClientRpc]のアトリビュートを付けることでRPCの対象メソッドとして指定することができます。

//emlist[][C]{
  [ServerRpc]
  private void TestServerRpc()
  {
    Debug.Log($"NoticeJoinedClientRpc");
  }

  [ClientRpc]
  private void TestClientRpc()
  {
    Debug.Log($"TestClientRpc");
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
今回の例ではゲームサーバーホスティング機能は使っていないため、見るべきはホスト(サーバー＋クライアント)とクライアントです。
ホストは、サーバーでありクライアントでもあるため、ServerRpc送信/実行とClientRpc送信/実行のすべてに対して権限があります。
一方でクライアントは、ServerRpcの実行とClientRpcの送信の権限がありません。
例えば、ホストA、クライアントB、クライアントCがいる状況で、クライアントBがクライアントCにならんかの処理を実行させたい場合には、
クライアントBはホストAのServerRpcメソッドを呼び出します。
そこからClientRPCを呼び出してクライアントCのメソッドを呼び出す様な処理が必要になります。

今回は『Multiplayer Samples Utilities』というライブラリを使用しています。2022年にUGSを使ったマルチプレイヤーゲーム開発のデモとして
『Boss Room」というプロジェクトがUnityから公開されており、そこで使用されているユーティリティライブラリです。
NGOではオブジェクトのTransform(位置、回転、サイズ)情報を同期するためにNetworkTransformというコンポーネントが元から用意されているのですが、
『Multiplayer Samples Utilities』にはNetworkTransformをクライアント用にラップしたクラスとしてClientNetworkTransformが存在します。
これをプレイヤーとなるオブジェクトにアタッチすることでプレイヤーの位置を同期できるようになります。


//indepimage[5_1_1_RPCについて][各RPCの権限について]{
//}


PlayersLinkでは、PlayerオブジェクトにClietn

==== ServerRpc

RequireOwnership

==== ClientRpc


==== Network Transform
==== Network Animation
Network Variable 