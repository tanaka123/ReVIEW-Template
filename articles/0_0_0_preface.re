= 前書き

本書をお手に取っていただき誠にありがとうございます。

AnoGameという名前で活動していて、個人でゲーム開発をしております。
もともとは金融系システムを主に扱う会社のプログラマとして勤務していました。2013年に新卒で入社して1年目の終わり頃に会社の先輩からゲームエンジンのUnityのことを教えてもらい、
そこからのめり込む様に暇な時間があればゲーム制作する様な生活が始まりました。

2022年7月、『PieceWars』というゲームをリリースして、現在はそのオンライン版の開発に取り組んでおります。
その傍らでオンライン対戦の実装を簡易にできるツールがあったらいいなぁという思いから『PlayersLink』の構想が浮かび上がりました。

『PlayersLink』は、Unity上で動作する文字通りプレイヤーを繋ぐシステムです。
UnityからUnity Gaming Servicies(以下、UGS)というサービスが提供されており、その中のLobbyとRelayというシステムを使って実現しています。
本書では、Unityを多少なりとも触ったことのある読者を想定しており、
『PlayersLink』の使い方と仕組みを解説していくことでUGSへの理解を深め、読者が最短経路でオンラインゲームを開発できるようになることを目指します。


== 免責事項

本書に記載された内容は、情報の提供のみを目的としています。
したがって、本書を用いた開発、製作、運用は、必ずご自身の責任と判断によって行ってください。
これらの情報による開発、製作、運用の結果について、著者はいかなる責任も負いません。
