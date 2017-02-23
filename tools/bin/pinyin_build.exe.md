# pinyin_build.exe

## Links
* [nark.cc 首页即拼音纠错 demo](http://nark.cc)

## Usage Help
```text
Usage:
   pinyin_build.exe Options [Input-TXT-File]

Description:
   Build pinyin-to-HanZiWord DFA from Input-HanZiWord-File,
   If Input-HanZiWord-File is omitted, use stdin.

Options:
   -h : Show this help infomation
   -q : Be quiet, don't print progress info
   -O Large-DFA-File : large, but pretty fast
   -o Small-DFA-File : small, but pretty slow, now deprecated
   -S Super-DFA-File : small, a little (smaller and slower) than Quick-DFA
   -U Louds-DFA-File : very small, very slow(~5x slower than Large-DFA)
   -u Louds-DFA-File : same as -U, but use RankSelect_IL
   -m [Save-As-MemMap]: Argment is optional, default is 1
       * If not specified this option or omitted the argument, use default(1)
       * If the argument is 0, it means "DO NOT Save-As-MemMap"
   -z MaxZpathTrieNum:MinZpathLen : default is 5:2
   -E u1:e1,u2:e2,u3:e3,...
      Build edit-distance keys into the result DFA
      u1:e1 indicate edit-distance 'e1' is tolerated for unicode char num 'u1'
      Root of edit-distance is dfa.state_move(initial_state, '\1')
   -H : With HanZiWord, the result DFA is not only searched by PinYin, but aslo
      the HanZiWord self, a HanZiWord may have satellite data(such as word-freq),
      this will enable searching the satellite data by HanZiWord
   -j MinJianPinLen : default is 7
      Build JianPin(just ShengMu) for HanZiWords which length is at least
      MinJianPinLen
   -p BasePinYin-File
      * BasePinYin should at least include all single HanZi's PinYin.
      * BasePinYin could include extra HanZiWord to PinYin pairs, this feature
        is for prevent auto-spell Multiple (Pinyin to HanZiWord) pairs.
   -2 : Allow for Double-ShengMu JianPin
        A JianPin of a HanZiWord should be All-Double-ShengMu or All-Single-ShengMu
   -w WarningType
      Only one WarningType(nohz) is supportted now. More warning types maybe
      added later.
      WarningTypes:
      * nohz : Warning when unicode chars of word are not all HanZi
```
