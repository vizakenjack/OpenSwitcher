//
//  Consts.swift
//  OpenSwitcher
//
//  Created by v1z on 01.12.2022.
//

import Foundation

struct Consts {
    static let escKey: UInt16 = 53
    static let spaceKey: UInt16 = 49
    static let tabKey: UInt16 = 48
    static let optionKey: UInt16 = 58
    static let controlKey: UInt16 = 59
    static let commandKey: UInt16 = 55
    static let backspaceKey: UInt16 = 51
    static let shiftKey: UInt16 = 56
    static let returnKey: UInt16 = 36
    static let fnKey: UInt16 = 179
    static let capslockKey: UInt16 = 57
    
    static let resetKeys: [UInt16] = [36, 123, 124, 125, 126]
    
    static let keycodes: [Int64: String] = [
        0: "a",
        1: "s",
        2: "d",
        3: "f",
        4: "h",
        5: "g",
        6: "z",
        7: "x",
        8: "c",
        9: "v",
        10: "§",
        11: "b",
        12: "q",
        13: "w",
        14: "e",
        15: "r",
        16: "y",
        17: "t",
        18: "1",
        19: "2",
        20: "3",
        21: "4",
        22: "6",
        23: "5",
    //    24: "",
        25: "9",
        26: "7",
    //    27: "",
        28: "8",
        29: "0",
        30: "]",
        31: "o",
        32: "u",
        33: "[",
        34: "i",
        35: "p",
    //    36: "",
        37: "l",
        38: "j",
        39: "'",
        40: "k",
        41: ";",
        42: "\\",
        43: ",",
        44: "/",
        45: "n",
        46: "m",
        47: ".",
        48: "   ",
        49: "Space",
        50: "`",
        123: "ArrowLeft",
        124: "ArrowRight",
        125: "ArrowDown",
        126: "ArrowUp",
    ]
    
    static let modifiers: [Int64: String] = [
        36: "Return",
        49: "Space",
        53: "Esc",
        51: "Backspace",
        56: "Shift",
        57: "Capslock",
        55: "Command",
        58: "Option",
        59: "Control",
        179: "Fn",
    ]
    
}

