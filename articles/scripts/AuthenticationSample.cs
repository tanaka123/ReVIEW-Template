#@mapfile(scripts/hello.rb)
#!/usr/bin/env ruby

using UnityEngine;
using System.Threading;
using System.Threading.Tasks;
using Unity.Services.Lobbies.Models;
using Unity.Netcode;
using UnityEngine.Events;

using System;
using System.Collections.Generic;
using Unity.Services.Authentication;
using Unity.Services.Core;

public class AuthenticationSample
{
  public async Task SignInAnonymouslyAsync()
  {
      try
      {
          await AuthenticationService.Instance.SignInAnonymouslyAsync();
          Debug.Log("認証成功");
          
          // Shows how to get the playerID
          Debug.Log($"PlayerID: {AuthenticationService.Instance.PlayerId}"); 

      }
      catch (AuthenticationException ex)
      {
          // Compare error code to AuthenticationErrorCodes
          // Notify the player with the proper error message
          Debug.LogException(ex);
      }
      catch (RequestFailedException ex)
      {
          // Compare error code to CommonErrorCodes
          // Notify the player with the proper error message
          Debug.LogException(ex);
      }
  }
}
#@end