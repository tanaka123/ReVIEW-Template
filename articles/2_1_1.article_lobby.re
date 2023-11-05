= Lobbyについて
この章ではLobby機能について実際のコードを見ながら説明していきます。

== 認証
先にRelayと共通で実施することになる認証処理についても見ていきます。

=== 認証処理の最小のサンプル
下記は、ロビー作成の例です。

//emlist[認証処理サンプル][C]{
using System.Threading.Tasks;
using UnityEngine;
using Unity.Services.Core;
using Unity.Services.Authentication;

public class AuthenticationSample
{
  public async Task SignInAnonymouslyAsync()
  {
    // Unityサービスが初期化されていなければ初期化を実行
    if (UnityServices.State == ServicesInitializationState.Uninitialized)
    {
      await UnityServices.InitializeAsync();
    }

    // 匿名認証
    await AuthenticationService.Instance.SignInAnonymouslyAsync();
    Debug.Log("認証成功");

    // プレイヤーIDを表示
    Debug.Log($"PlayerID: {AuthenticationService.Instance.PlayerId}"); 
  }
}
//}
Unityサービスの初期化後、SignInAnonymouslyAsyncメソッドを非同期で実行するだけです。
エラーハンドリングについては、下記の様にAuthenticationExceptionで認証処理自体のエラー、
RequestFailedExceptionでリクエスト失敗時のエラーをハンドリングしてあげればいいです。

//emlist[認証処理サンプル２][C]{
public class AuthenticationSample
{
  public  async Task SignInAnonymouslyAsync()
  {
    try
    {
      await AuthenticationService.Instance.SignInAnonymouslyAsync();
      Debug.Log("認証成功");

      // プレイヤーIDを表示
      Debug.Log($"PlayerID: {AuthenticationService.Instance.PlayerId}"); 

    }
    catch (AuthenticationException ex)
    {
      // 認証エラー
      Debug.LogException(ex);
    }
    catch (RequestFailedException ex)
    {
      // リクエストエラー
      Debug.LogException(ex);
    }
  }
}
//}

また、認証サービスへのサインイン、サインアウトのイベントは下記の様に購読することができます。

//emlist[認証処理のイベント購読][C]{
AuthenticationService.Instance.SignedIn += OnLoggedInHandler;
AuthenticationService.Instance.SignedOut += OnLoggedOutHandler;
//}

@<embed>{|latex|\pagebreak }

== Lobbyの処理
認証処理が完了した後は、LobbyとRelayのいずれも使用可能になります。

=== ロビーの最小のサンプル
下記は、ロビー作成/参加処理の例です。

//emlist[ロビー作成サンプル][C]{
using Unity.Services.Lobbies;

public class LobbySample
{
  private async Task CreateLobby()
  {
    string lobbyName = "Test";
    int playerCount = 2;

    // ロビーを作成
    await Lobbies.Instance.CreateLobbyAsync(lobbyName, playerCount);

    await SendLobbyHeartbeat(lobbyName, 5);
  }

  // ハートビート処理
  private async void SendLobbyHeartbeat(string lobbyId, int intervalSeconds)
  {
    var delayMilliSeconds = intervalSeconds * 1000;
    while (true)
    {
      // 待機
      await UniTask.Delay(delayMilliSeconds);

      // ロビー接続を継続する
      try
      {
          await Lobbies.Instance.SendHeartbeatPingAsync(lobbyId);
      }
      catch (Exception e)
      {
          Debug.LogError($"ハードビート失敗:{e}");
          break;
      }
    }
  }
}
//}

lobbyNameにはロビーの名前、playerCountにはロビーに入室可能な最大人数を設定します。
これでロビーを作成できます。
SendLobbyHeartbeatという処理がありますが、これはロビーへの接続維持を行っています。
Lobbyデフォルトの設定では、30秒以上ハートビートが送られてこなければホストが切断したものと見なされてロビーが解散されます。
サンプルでは5秒に1回Lobbyにハートビートを送る処理になっています。

===[column] Unityあるある

上記のようなハートビート処理の様に、明らかに必須の処理であるにも関わらず「え、こっちで実装するの？」っていういつものパターンですね。

===[/column]


ロビーに参加する側は下記を実行します。
//emlist[ロビー参加サンプル][C]{
public class LobbySample
{
  private async UniTask JoinLobby()
  {
    await Lobbies.Instance.QuickJoinLobbyAsync();
  }
}
//}

これにより、現時点で存在するロビーに入ることができます。
あくまでクイックジョインなので狙った部屋に入室したい場合は、QueryLobbiesAsyncメソッドを使ってロビーの検索を行う必要があります。
この時、ロビー作成時にQueryLobbiesOptionsというオプションをQuickJoinLobbyAsyncメソッドの引数として持たせてあげる必要があります。
また、QueryLobbiesOptionsにはQueryFilterというフィルター情報があるので、ここに細かな条件を追加していくことでロビー参加時にも
QueryLobbiesAsyncメソッドを使う際にフィルター情報で絞り込むことができます。
その他にも、JoinLobbyByIdAsync、JoinLobbyByCodeAsyncメソッドが存在しており、作成したロビーからIDまたはCodeを取得することができるので
これを他のユーザーに共有することで参加することができます。おそらく、これはプライベートマッチを想定しての機能と思われます。


===[column] QueryFilterにを使ったフィルターの活用

QueryFilterは、キーバリュー型のデータとして持たせることができるので、レベルや戦績で使ってレーティングシステムを作ることもできます。
また、協力型のゲームであれば、現在進行中のステージ情報の保持に役立ちます。

===[/column]

== LobbyData
作成したロビーには、LobbyDataを持たせることができます。
具体的なコードは下記の通りです。

//emlist[LobbyData書き込み][C]{
public class LobbySample
{
  public async Task UpdateLobbyData(string key, string value)
  {
    Dictionary<string, DataObject> data = new Dictionary<string, DataObject>()
    {
      {
        key, new DataObject(DataObject.VisibilityOptions.Public, value)
      }
    };

    var updateLobbyOptions = new UpdateLobbyOptions()
    {
      Data = data,
    };
    await Lobbies.Instance.UpdateLobbyAsync(lobbyId, updateLobbyOptions);
  }
}
//}

前提として、作成/参加したロビーのIDを保持しておく必要があります。
Dictionary<string, DataObject>型のデータを作成して、UpdateLobbyOptionsのデータとして持たせます。
UpdateLobbyAsyncメソッドを使い、更新するロビーIDを指定してUpdateLobbyOptionsで上書くといった感じです。
QueryFilterと同じように、キーバリュー型のデータを持たせることができるので各ゲームに応じて必要な情報を持たせることができます。
なお、LobbyDataの書き込み権限はロビー作成者であるホストプレイヤーのみが持っています。


PlayersLinkでは、LobbyDataについてはJoinコードの読み書きでしか使用しないため、
UpdateLobbyOptionsのデータとしては1つのデータしか持たせていません。
下記の様にJoinコードを保存する処理で呼び出して使っています。

//emlist[LobbyData書き込み(Joinコード)][C]{
public class LobbySample
{
  public async Task SaveJoinCode(string joinCode)
  {
    await UpdateLobbyData("JoinCode", joinCode);
  }
}
//}

また、LobbyDataにアクセスする場合は、GetLobbyAsyncでロビー情報を取得して、LobbyDataを読み込みます。
PlayersLinkでは下記の様にJoinコードを読み込んでいます。

//emlist[LobbyData読み込み(Joinコード)][C]{
public class LobbySample
{
  public string GetJoinCode()
  {
    var lobby = await Lobbies.Instance.GetLobbyAsync(lobbyId);
    var joinCode = lobby.Data["JoinCode"].Value;
    return joinCode;
  }
}
//}

===[column] .Value

通常のDictionaryであれば、dict["key"]で値を取得できるはずなのだが、これまで見てきたコードでは
データ取得時に「.Value」で取得していることに注意してください。
開発中に「なんか値取れないなぁ」と何時間も格闘しないためにも覚えておいていただけると幸いです。

===[/column]

== PlayerData
Lobbyには、LobbyDataの他にPlayersというプロパティがあります。
このPlayersはPlayerクラスのリストとなっており、PlayerにもDataを保持しておくことができます。
下記はPlayerDataへの書き込み方法です。

//emlist[PlayerData書き込み][C]{
public class LobbySample
{
  public void CheckPlayerData()
  private async Task WriteConnectionTimestamp(Lobby lobby, string playerId)
  {
      var playerData = GetPlayersData(lobby, playerId);
      if (playerData != null)
      {
        // タイムスタンプデータがあれば削除
        if (playerData.ContainsKey(LobbyConstants.ConnectionTimestamp))
        {
          playerData.Remove(LobbyConstants.ConnectionTimestamp);
        }

        // 現時刻のタイムスタンプを取得する
        string nowTimeStamp = DateTime.Now.ToString(LobbyConstants.ConnectionTimestampFormat);

        // プレイヤーデータに接続タイムスタンプを追加
        playerData.Add(LobbyConstants.ConnectionTimestamp, new PlayerDataObject(PlayerDataObject.VisibilityOptions.Public, nowTimeStamp));

      }

      // アップデートプレイヤーオプションを作成
      var updatePlayerOptions = new UpdatePlayerOptions()
      {
        Data = playerData
      };

      // プレイヤーデータをアップデートする
      await Lobbies.Instance.UpdatePlayerAsync(lobby.Id, playerId, updatePlayerOptions);  
  }

  private  Dictionary<string, PlayerDataObject> GetPlayersData(Lobby lobby, string playerId)
  {
    var player = lobby.Players.FirstOrDefault(x => x.Id == playerId);
    if (player.Data != null)
    {
      return player.Data;
    }
    else
    {
      return null;
    }
  }
}
//}

PlayersLinkではプレイヤーのタイムスタンプ情報をやり取りすることで疎通確認できるようするため、
上記の様なタイムスタンプの書き込み処理を行っています。(この背景などについては後述します。)
LobbyのPlayersの中から書き込み対象のプレイヤーIDを指定してDataを得るため、GetPlayersDataメソッドの処理を行っています。
コードが長くなっていますが、重複したデータを登録しない様に制御しています。
最終的にUpdatePlayerAsyncでロビーIDとプレイヤーIDを指定して、UpdatePlayerOptionsを更新するという流れです。

次に、下記はPlayerDataの読み込み方法です。

//emlist[PlayerData読み込み][C]{
public class LobbySample
{
  public void CheckPlayerData()
  {
    foreach (var lobbyPlayer in lobby.Players)
    {
      var ConnectionTimestamp =  lobbyPlayer.Data["ConnectionTimestamp"].Value;

      // ...中略...
    }
  }
}
//}

このあたりはLobbyDataとやることはほとんど一緒でPlayersのDataにアクセスするだけです。



