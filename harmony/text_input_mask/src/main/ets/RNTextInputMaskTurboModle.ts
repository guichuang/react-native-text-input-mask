import { TurboModule, TurboModuleContext } from '@rnoh/react-native-openharmony/ts';

export class RNTextInputMaskTurboModule extends TurboModule  {
  private context: TurboModuleContext;

  constructor(ctx: TurboModuleContext) {
    super(ctx)
    this.context = ctx
  }

  mask(mask: string, value: string, autocomplete: boolean): Promise<string> {
    return;
  }

  unmask(mask: string, value: string, autocomplete: boolean): Promise<string> {
    return;
  }

  setMask(reactNode: number, primaryFormat: string, options: object): void {
    console.log("==================", "setMask")
  }

}