= 同期処理
この章では、Netcode for GameObjectsを使った同期処理について説明していきます。

== Netcode for GameObjectsについて
Relayサーバーを経由してホストとクライアントの接続が完了するとNetcode for GameObjects(以下、NGO)という
ライブラリを使って同期処理を行うことができます。
すでにRelayの章でも登場していたNetworkManagerもNGOのコンポーネントです。
NetworkManagerは、先に見た通りホスト(サーバー)とクライアントの接続を管理しています。
実際に同期させるオブジェクトには、NetworkObjectをアタッチします。

//indepimage[5_1_1_PlayerObject][NetworkObject]{
//}

PlayersLinkでは、同期させる人型オブジェクトにNetworkObjectをアタッチしています。
このプレイヤーとなるキャラクターのプレハブをNetworkManagerのPlayerPrefabにセットしています。

//indepimage[5_1_2_NetworkManager_PlayerPrefab][NetworkObject]{
//}

こうすることで、NetworkManagerのStartHost、StartClientが実行されると同時にキャラクタープレハブがシーン上にスポーンされます。
PlayersLinkでは、Unityで無料配布されている『Starter Assets - Third Person Character Controller』というアセットの
PlayerArmatureをキャラクタープレハブとして使用しています。
下図は、PlayersLinkでマッチング完了後に始まるメインゲームのサンプルです。

//indepimage[5_1_3_PlayersLinkのサンプルゲーム][NetworkObject]{
//}

建物の上にあるゴールを目指すシンプルなゲームです。


//indepimage[5_1_4_PlayersLinkのサンプルゲーム2][NetworkObject]{
//}

ゴール後、ロビーに戻るためのポップアップが表示されます。
ロビーに戻るときにNetworkManagerのホスト/クライアントの停止が行われることでRelayサーバーは削除されます。


== RPCについて

RPCは『Remote Procedure Call』の略です。平たく言えば、他の端末のメソッドを実行できる機能です。
NGOでは、下記の様に[ServerRpc]または[ClientRpc]のアトリビュートを付けることでRPCの対象メソッドとして指定することができます。

//emlist[][C]{
public class SampleRPC : NetworkBehaviour
{
    [ServerRpc]
    private void TestServerRpc()
    {
    Debug.Log($"TestServerRpc");
    }

    [ClientRpc]
    private void TestClientRpc()
    {
    Debug.Log($"TestClientRpc");
    }
}
//}


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
『Boss Room』というプロジェクトがUnityから公開されており、そこで使用されているユーティリティライブラリです。
NGOではオブジェクトのTransform(位置、回転、サイズ)情報を同期するためにNetworkTransformというコンポーネントが元から用意されているのですが、
『Multiplayer Samples Utilities』にはNetworkTransformをクライアント用にラップしたコンポーネントとしてClientNetworkTransformが存在します。
これをプレイヤーとなるオブジェクトにアタッチすることでプレイヤーの位置を同期できるようになります。
ClientNetworkAnimatorというコンポーネントも提供されており、これはAnimatorのアニメーション同期に使用します。


== NetworkVariableについて
RPCの他にデータの共有方法として、NetworkVariableという機能があります。
これを使用すると変数の値が変更されたときにその通知を受け取ることができます。

//emlist[][C]{
public class SampleNetworkVariable : NetworkBehaviour
{
    private NetworkVariable<int> m_SomeValue = new NetworkVariable<int>(0);

    public override void OnNetworkSpawn()
    {
        m_SomeValue.OnValueChanged += OnSomeValueChanged;
    }

    private void OnSomeValueChanged(int previous, int current)
    {
        Debug.Log($"変更前の値:{previous}…現在の値:{current}");
    }
}
//}

NetworkBehaviourを継承したコンポーネントをアタッチしたオブジェクトがスポーンされるとOnNetworkSpawnが呼び出されます。
この時、NetworkVariable<int>型のm_SomeValueのイベントを購読することができ、m_SomeValueの値が変更されるたびに登録したメソッドを呼び出すことができます。
ReactivePropertyに似たような挙動をするので、Webアプリ開発やUniRx使用した経験がある方にはなじみがあるかもしれません。
NetworkVariableの用途としては、プレイヤーのHPやスコアの共有が考えられます。
NetworkVariableとRPCの使い分けについて公式ドキュメントの記述によると、後から入室してきたプレイヤーに渡したい情報であればNetworkVariableを使い、
ゲーム内の突発的なイベントについてはRPCを使うことを推奨しているようでした。


