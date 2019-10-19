
#include "DialogueWidget.h"

void UDialogueWidget::BindDialogueComponent(class UDialogueSystemComponent* _DialogueComponent)
{
	DialogueComponent = _DialogueComponent;
}

void UDialogueWidget::OnSentenceAppear()
{
	Receive_OnSentenceAppear();

}

void UDialogueWidget::OnSentenceDisappear()
{
	Receive_OnSentenceDisappear();

}

void UDialogueWidget::OnSentenceChange()
{
	Receive_OnSentenceChange();

}
