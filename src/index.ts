import HarmonyTextInputMask ,{MaskOptions}from './index.harmony'
import {mask as maskA, unmask as unmaskA, setMask as setMaskA }  from 'react-native-text-input-mask';
import { Platform } from 'react-native';

interface MaskOperations {
    mask(mask: string, value: string, autocomplete: boolean): Promise<string> ;
    unmask(mask: string, value: string, autocomplete: boolean): Promise<string> 
    setMask(reactNode: number, primaryFormat: string, options?: MaskOptions): void
}
const isIosAndroid = Platform.OS === 'ios' || Platform.OS === 'android';


class RNTextInputMask {
constructor(){}
    static mask(mask: string, value: string, autocomplete: boolean): Promise<string> {
            return maskA(mask, value, autocomplete);
        }
    
        static unmask(mask: string, value: string, autocomplete: boolean): Promise<string> {
            return unmaskA(mask, value, autocomplete);
        }
        static  setMask(reactNode: number, primaryFormat: string, options?: MaskOptions): void {
        setMaskA(reactNode, primaryFormat, options)
        }
}
console.log("======isIosAndroid=",isIosAndroid)
console.log("======RNTextInputMask=",RNTextInputMask)
export const exportMasker:MaskOperations= isIosAndroid ? RNTextInputMask : HarmonyTextInputMask
console.log("======exportMasker=",exportMasker)
export default exportMasker;
