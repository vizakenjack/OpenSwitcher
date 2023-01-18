//
//  ContentView.swift
//  OpenSwitcher
//
//  Created by v1z on 01.12.2022.
//

import SwiftUI

struct ContentView: View {
    @AppStorage("hotkeyCode") private var hotkeyCode = 58
    @AppStorage("hotkeyName") private var hotkeyName = "Option"
    @AppStorage("languageKeyCodes") private var languageKeyCodes = "59,49"
    @AppStorage("languageKeyNames") private var languageKeyNames = "Control + Space"
    @State var isChangingSwitchKey: Bool = false
    @State var isChangingLanguageKey: Bool = false
   
    var isAuthorized: Bool = false

    init(isAuthorized: Bool) {
        self.isAuthorized = isAuthorized
        let languageKeyCodesArray = languageKeyCodes.split(separator: ",").compactMap{Int64(String($0))}
        keyManager.setSwitchKey(hotkeyCode)
        keyManager.setLanguageKeys(languageKeyCodesArray)
    }
    
    var body: some View {
        VStack {
            HStack {
                Text("Manual switch:")
                Button("\(isChangingSwitchKey ? "Changing" : hotkeyName)", action: setKey)
            }
            HStack {
                Text("Language change:")
                Button("\(isChangingLanguageKey ? "Changing" : languageKeyNames)", action: setLanguageKey)
            }
            Spacer()
                .frame(height: 10)

            Text("Access: \(isAuthorized)" as String)
          
        }
        .padding()
        .frame(minWidth: 300.0, minHeight: 200.0)
    }
    
    func setKey() {
        if (self.isChangingLanguageKey) {
            return
        }
        
        keyManager.switchEnabled = false
        self.isChangingSwitchKey = true
        
        Task {
            defer {
                keyManager.switchEnabled = true
            }
            
            let keyCodes = await keyManager.waitForKeys()
            if (keyCodes.isEmpty) {
                return
            }
            
            let keyCode = keyCodes[0]
            print("Got: \(keyCode)")
            
            if (keyCodes[0] == Consts.escKey) {
                print("Got esc")
                self.isChangingSwitchKey = false
                return
            }
            
            let languageKeyCodesArray = languageKeyCodes.split(separator: ",").compactMap{Int64(String($0))}
            
            if languageKeyCodesArray.count == 1 && languageKeyCodesArray[0] == keyCode {
                self.languageKeyCodes = ""
                self.languageKeyNames = "None"
            }
            
            if let keyName = Consts.modifiers[keyCode] {
                self.isChangingSwitchKey = false
                self.hotkeyName = keyName
                self.hotkeyCode = Int(keyCode)
                keyManager.setSwitchKey(self.hotkeyCode)
            }
            
        }
    }
    
    func setLanguageKey() {
        if (self.isChangingSwitchKey) {
            return
        }
        keyManager.switchEnabled = false
        self.isChangingLanguageKey = true
        
        Task {
            defer {
                self.isChangingLanguageKey = false
                keyManager.switchEnabled = true
            }
            
            let keyCodes = await keyManager.waitForKeys()
            print("WAITED! Got: \(keyCodes)")
            
            if (keyCodes.isEmpty) {
                return
            }

            if (keyCodes[0] == Consts.escKey) {
                print("Got esc")
                self.isChangingSwitchKey = false
                return
            }
            
            if keyCodes.count == 1 && keyCodes[0] == self.hotkeyCode {
                self.hotkeyCode = -1
                self.hotkeyName = "None"
            }
            
            self.languageKeyNames = keyCodes.compactMap{Consts.modifiers[$0]}.joined(separator: " + ")
            self.languageKeyCodes = keyCodes.compactMap{String($0)}.joined(separator: ",")
            print("languageKeyCodes is \(languageKeyCodes)")
            keyManager.setLanguageKeys(keyCodes)
        }
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView(isAuthorized: true)
    }
}
