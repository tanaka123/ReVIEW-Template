
= Relayについて
この章ではRelay機能について話していきます。

== 認証
@<chapref>{3_1_1.article_lobby}の認証と同様の手順で認証を行います。

== Relayの処理
認証処理が完了したらRelayが使用可能になります。

@<embed>{|latex|\pagebreak }

=== Relayの最小サンプル

下記はRelayサーバー作成処理の例です。

//emlist[][C]{
public class RelaySample
{
  public async Task CreateRelay()
  {
    int maxConnections = 2;
    var allocation = await RelayService.Instance.CreateAllocationAsync(maxConnections);

    var joinCode = await RelayService.Instance.GetJoinCodeAsync(allocation.AllocationId);

    if (NetworkManager.Singleton.NetworkConfig.NetworkTransport is UnityTransport utp)
    {
#if UNITY_WEBGL
        // WebGLの場合は、WSSを使う
        var relayServerData = new RelayServerData(allocation, "wss");
        utp.UseWebSockets = true;
# else
        var relayServerData = new RelayServerData(allocation, "dtls");
#endif
        utp.SetRelayServerData(relayServerData);
        NetworkManager.Singleton.StartHost();
        // ...中略...
    }
  }
}
//}


参加できる最大人数を設定してCreateAllocationAsyncメソッドを実行します。これにより、Relayサーバーを作成することができます。
また、アロケーションIDを指定してGetJoinCodeAsyncメソッドを実行することでJoinコードを取得することができるので、
これをLobbyDataの読み書き等で使用するために保持しておきます。
次にUnityTransportの設定を行います。ここでは、通信に使用するプロトコルを設定してStartHostメソッドを実行しています。
"dtls"プロトコルはどのプラットフォームでも動作しますが、WebGLではWebソケット通信を行う必要があるため、
"wss"プロトコルを使うような切り替えを行っています。
アロケーション情報とプロトコル情報を使ってRelayServerData作成し、UTPにRelayServerDataを設定しています。
NetworkManagerのStartHostを実行することで、実行した端末がサーバーとなり同時にホストとしての処理を開始します。

#@#TODO:シーン上のNetworkManagerとUTPの構成を載せた方がいいか？

@<embed>{|latex|\pagebreak }

下記はRelayサーバー参加処理の例です。

//emlist[][C]{
public class RelaySample
{
  public async Task JoinRelay(string joinCode)
  {
    var joinAllocation = await RelayService.Instance.JoinAllocationAsync(joinCode);

    if (NetworkManager.Singleton.NetworkConfig.NetworkTransport is UnityTransport utp)
    {
#if UNITY_WEBGL
        // WebGLの場合は、WSSを使う
        var relayServerData = new RelayServerData(joinAllocation, "wss");
        utp.UseWebSockets = true;
# else
        var relayServerData = new RelayServerData(joinAllocation, "dtls");
#endif
        utp.SetRelayServerData(relayServerData);
        NetworkManager.Singleton.StartClient();
        // ...中略...
    }
  }
}
//}


クライアント側はLobbyDataを経由してJoinコードを取得します。(必ずしもLobbyDataとは限りませんが。)
次にJoinコードを指定してJoinAllocationAsyncメソッドを実行しています。
これによりアロケーション情報を取得することができます。あとはホスト側と同じようにプロトコル設定を行い、
NetworkManagerのStartClientメソッドを実行することでホスト側の端末に接続することことができます。

お疲れ様でした！ここまでできればプレイヤー同士の接続は完了です。
次はいよいよ同期処理について見ていきます。
