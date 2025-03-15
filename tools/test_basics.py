import pytest
import time

def test_connection(ft_irc_server, ngircd_server, irc_client):
    """基本的な接続テスト"""
    # ft_ircテスト
    ft_client = irc_client("localhost", ft_irc_server.port)
    ft_client.connect()
    ft_client.send_raw("PING :test")
    ft_response = ft_client.receive()
    ft_client.disconnect()
    
    # ngircdテスト
    ng_client = irc_client("localhost", ngircd_server.port)
    ng_client.connect()
    ng_client.send_raw("PING :test")
    ng_response = ng_client.receive()
    ng_client.disconnect()
    
    # レスポンス比較
    assert ft_response, "ft_ircからレスポンスが返りませんでした"
    assert ng_response, "ngircdからレスポンスが返りませんでした"
    
    # 完全一致は期待せず、PINGに対するPONGが含まれているかを検証
    assert any("PONG" in resp for resp in ft_response), "ft_ircからのPONGレスポンスがありません"
    assert any("PONG" in resp for resp in ng_response), "ngircdからのPONGレスポンスがありません"

def test_registration(ft_irc_server, ngircd_server, irc_client):
    """ユーザー登録テスト"""
    # ft_ircテスト
    ft_client = irc_client("localhost", ft_irc_server.port)
    ft_client.connect()
    ft_client.password = ft_irc_server.password
    ft_response = ft_client.register("test_user1")
    ft_client.disconnect()
    
    # ngircdテスト
    ng_client = irc_client("localhost", ngircd_server.port)
    ng_client.connect()
    ng_client.password = ngircd_server.password
    ng_response = ng_client.register("test_user1")
    ng_client.disconnect()
    
    # 登録成功の検証
    assert ft_response, "ft_ircからのレスポンスがありません"
    assert ng_response, "ngircdからのレスポンスがありません"
    
    # ユーザー登録成功メッセージの検証（サーバーによって異なる場合があるため厳密な比較はしない）
    assert any("001" in resp for resp in ft_response) or any("Welcome" in resp for resp in ft_response), \
        "ft_ircからの登録成功メッセージがありません"
    assert any("001" in resp for resp in ng_response) or any("Welcome" in resp for resp in ng_response), \
        "ngircdからの登録成功メッセージがありません"