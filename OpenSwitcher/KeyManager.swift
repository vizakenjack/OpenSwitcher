//
//  KeyManager.swift
//  OpenSwitcher
//
//  Created by v1z on 01.12.2022.
//

import Foundation
import Carbon

class KeyManager {
    var switchEnabled = true
    private var switchKey = Consts.optionKey
    private var languageKeys: [UInt16] = []
    
    private var isPressing = false
    private var isSwitching = false
    private var waitingForKey = false
    private var waitingForHotkeyRelease = false
    private var waitingForNextSymbolAfterSpace = false
    
//    private var lastKeyPressed: Int64 = -1
    private var lastKeysPressed: [Int64] = []
    private var hotkeysPressed: Int8 = 0
    private var keyPressedAt: Double = 0
    
    private var bufferKeys: [UInt16] = []
    private let delay: UInt64 = 20_000_000

    func onKeyUp(_ keyCode: Int64, flags: UInt64) {
        if (isSwitching) {
            return
        }
        
        if (keyCode >= 256) {
            print("Unknown keycode: \(keyCode)")
            return;
        }
        
        let commandPressed = flags & CGEventFlags.maskCommand.rawValue != 0 && keyCode != 54 && keyCode != 55
        
        let int16code = UInt16(truncatingIfNeeded: keyCode)
        keyPressedAt = CFAbsoluteTimeGetCurrent()
        
        // check for command+A
        if (keyCode <= 50 && commandPressed) {
            print("Got a command, clearing: \(bufferKeys)")
            clearBuffer()
            return
        }
        
        if keyCode == Consts.spaceKey {
            if (waitingForKey) {
//                lastKeyPressed = keyCode
                if (!lastKeysPressed.contains(keyCode)) {
                    lastKeysPressed.append(keyCode)
                }
            } else {
                waitingForNextSymbolAfterSpace = true
                bufferKeys.append(int16code)
            }
        } else if keyCode == Consts.backspaceKey {
            if (!bufferKeys.isEmpty) {
                bufferKeys.removeLast()
            }
        } else if (int16code == Consts.escKey) {
            if (waitingForKey) {
//                lastKeyPressed = keyCode
                if (!lastKeysPressed.contains(keyCode)) {
                    lastKeysPressed = [keyCode]
                }
            } else {
                clearBuffer()
            }
        } else if (Consts.resetKeys.contains(int16code)) {
            clearBuffer()
        } else {
            if Consts.keycodes[keyCode] != nil {
                if (waitingForNextSymbolAfterSpace) {
                    clearBuffer()
                    waitingForNextSymbolAfterSpace = false
                }
                bufferKeys.append(int16code)
            }
        }
        print("keyCode is \(keyCode)")
    }
    
    func onReceiveFlag(_ keyCode: Int64) {
        if (isSwitching) {
            return
        }
        print("onReceiveFlag code is \(keyCode)")
        
        if (waitingForKey) {
            keyPressedAt = CFAbsoluteTimeGetCurrent()
//            lastKeyPressed = keyCode
            if (!lastKeysPressed.contains(keyCode)) {
                lastKeysPressed.append(keyCode)
            }
            return
        }
        
        if keyCode == Consts.commandKey {
            clearBuffer()
        }
        
        if keyCode == switchKey {
            hotkeysPressed += 1
            if (hotkeysPressed != 2) {
                return
            }
            
            hotkeysPressed = 0
            Task {
                await switchKeyboard()
            }
        }
    }
    
//    func waitForKey() async -> Int64 {
//        waitingForKey = true
//
//        defer {
//            print("Defer")
//            waitingForKey = false
//            lastKeyPressed = -1
//        }
//
//        do {
//            for _ in 1...100 {
//                if (lastKeyPressed == -1) {
//                    try await Task.sleep(nanoseconds: 1_000_000_000)
//                } else {
//                    break
//                }
//
//            }
//
//        } catch {
//            print("Error")
//        }
//
//        return lastKeyPressed
//    }
    
    func waitForKeys() async -> [Int64] {
        waitingForKey = true

        defer {
            print("Clearing")
            waitingForKey = false
            lastKeysPressed = []
        }
       
        do {
            for _ in 1...100 {
                let diff = CFAbsoluteTimeGetCurrent() - keyPressedAt
                print("Diff is \(diff), lastKeysPressed is \(lastKeysPressed)")
                if (lastKeysPressed.isEmpty || diff < 0.5) {
                    try await Task.sleep(nanoseconds: 100_000_000)
                } else {
                    break
                }
            }
          
        } catch {
            print("Error")
        }
        
        
        
        return lastKeysPressed
    }
    
    func setSwitchKey(_ keyCode: Int) {
        self.switchKey = UInt16(truncatingIfNeeded: keyCode)
    }
    
    func setLanguageKeys(_ keyCodes: [Int64]) {
        self.languageKeys = keyCodes.compactMap{UInt16(truncatingIfNeeded: $0)}
    }
    
    func onMouseDown() {
        clearBuffer()
    }
    
    private func clearBuffer() {
        bufferKeys = []
    }
    
    private func switchKeyboard() async {
        if (!switchEnabled || bufferKeys.isEmpty || isSwitching) {
            print("Not switching")
            return
        }
        
        isSwitching = true
        
        defer {
            isSwitching = false
        }
        
        sendKeyUp()
        await sendBackspaces()
        switchLanguage()
        await sendKeys()
    }
        
    private func sendBackspaces() async {
        do {
            try await Task.sleep(nanoseconds: delay * 5) // 80ms
            for _ in 1...(bufferKeys.count) {
                keypress(Consts.backspaceKey)
            }
            
            try await Task.sleep(nanoseconds: delay * 2) // 40ms
        } catch {
            print("Error")
        }
    }
        
    private func sendKeys() async {
        do {
            try await Task.sleep(nanoseconds: delay * 2)
            
            for key in bufferKeys {
                keypress(key)
                try await Task.sleep(nanoseconds: delay)
            }
            
           
            try await Task.sleep(nanoseconds: delay) // 20ms
        } catch {
            print("Error")
        }
    }
    
    private func sendKeyUp() {
        let keyUp = CGEvent(keyboardEventSource: nil, virtualKey: switchKey, keyDown: false)
        keyUp?.post(tap: .cghidEventTap)

        CGEvent.init(source: nil)?.post(tap: .cghidEventTap)
    }

    private func keypress(_ keyCode: CGKeyCode) {
            let sourceRef = CGEventSource(stateID: .combinedSessionState)

            if sourceRef == nil {
                NSLog("FakeKey: No event source")
                return
            }

            let keyDownEvent = CGEvent(keyboardEventSource: sourceRef,
                                       virtualKey: keyCode,
                                       keyDown: true)

            let keyUpEvent = CGEvent(keyboardEventSource: sourceRef,
                                     virtualKey: keyCode,
                                     keyDown: false)

            keyDownEvent?.post(tap: .cghidEventTap)
            keyUpEvent?.post(tap: .cghidEventTap)
    }
    
    private func switchLanguage() {
        if (languageKeys.count == 1) {
            keypress(languageKeys[0])
        } else if (languageKeys.count == 2) {
            if (languageKeys.contains(Consts.spaceKey)) {
                guard let i = languageKeys.firstIndex(where: { $0 != Consts.spaceKey }) else {
                    print("Not found")
                    return
                }
                
                let spaceDown = CGEvent(keyboardEventSource: nil, virtualKey: Consts.spaceKey, keyDown: true)
                let spaceUp = CGEvent(keyboardEventSource: nil, virtualKey: Consts.spaceKey, keyDown: false)
    
                let secondKey = languageKeys[i]
                let modifierDown = CGEvent(keyboardEventSource: nil, virtualKey: secondKey, keyDown: true)
                let modifierUp = CGEvent(keyboardEventSource: nil, virtualKey: secondKey, keyDown: false)
                
                switch secondKey {
                case Consts.optionKey:
                    spaceDown?.flags = .maskAlternate
                    spaceUp?.flags = .maskAlternate
                case Consts.controlKey:
                    spaceDown?.flags = .maskControl
                    spaceUp?.flags = .maskControl
                case Consts.commandKey:
                    spaceDown?.flags = .maskCommand
                    spaceUp?.flags = .maskCommand
                case Consts.shiftKey:
                    spaceDown?.flags = .maskShift
                    spaceUp?.flags = .maskShift
                default:
                    print("Nothing found")
                }
                
                modifierDown?.post(tap: .cghidEventTap)
                spaceDown?.post(tap: .cghidEventTap)
                spaceUp?.post(tap: .cghidEventTap)
                modifierUp?.post(tap: .cghidEventTap)
        
                CGEvent.init(source: nil)?.post(tap: .cghidEventTap)
            }
        }
      
    }
}
