//
//  OpenSwitcherApp.swift
//  OpenSwitcher
//
//  Created by v1z on 01.12.2022.
//

import SwiftUI

let keyManager = KeyManager()

@main
struct OpenSwitcherApp: App {
    @AppStorage("showMenuBarExtra") private var showMenuBarExtra = true
    var isAuthorized: Bool = false
    
    init() {
        isAuthorized = checkPermissions()
     
        DispatchQueue.main.async {
            EventSource().start()
        }
    }
    
    func checkPermissions() -> Bool {
        let trusted = kAXTrustedCheckOptionPrompt.takeUnretainedValue()
        let privOptions = [trusted: true] as CFDictionary
        let accessEnabled = AXIsProcessTrustedWithOptions(privOptions)

        return accessEnabled
    }
    
    var body: some Scene {
        WindowGroup {
            ContentView(isAuthorized: isAuthorized)
        }
    }
    
  

}
