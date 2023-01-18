//
//  EventSource.swift
//  OpenSwitcher
//
//  Created by v1z on 01.12.2022.
//

import Foundation

import ApplicationServices

func myCGEventCallback(proxy: CGEventTapProxy, type: CGEventType, event: CGEvent, refcon: UnsafeMutableRawPointer?) -> Unmanaged<CGEvent>? {
    
//    print("type is \(type)")
    
    if type == .flagsChanged {
        let keyCode = event.getIntegerValueField(.keyboardEventKeycode)
        keyManager.onReceiveFlag(keyCode)
    } else if type == .keyUp {
        
        let keyCode = event.getIntegerValueField(.keyboardEventKeycode)
        keyManager.onKeyUp(keyCode, flags: event.flags.rawValue)
    } else if type == .leftMouseDown {
        keyManager.onMouseDown()
    }
    return Unmanaged.passRetained(event)
}

class EventSource {
    func start() {
        let eventMask = (1 << CGEventType.keyDown.rawValue) |
                        (1 << CGEventType.keyUp.rawValue) |
                        (1 << CGEventType.flagsChanged.rawValue)  |
                        (1 << CGEventType.leftMouseDown.rawValue)
        
        guard let eventTap = CGEvent.tapCreate(tap: .cgSessionEventTap,
                                               place: .headInsertEventTap,
                                               options: .defaultTap,
                                               eventsOfInterest: CGEventMask(eventMask),
                                               callback: myCGEventCallback,
                                               userInfo: nil) else {
                                                print("failed to create event tap")
                                                return
        }

        let runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0)
        CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, .commonModes)
        CGEvent.tapEnable(tap: eventTap, enable: true)
        CFRunLoopRun()
    }

}

