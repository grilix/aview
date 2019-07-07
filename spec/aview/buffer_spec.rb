require 'spec_helper'

describe AView::Buffer do
  let(:start) { '' }
  let(:buffer) { described_class.new(start) }

  subject { buffer.to_s }

  it { is_expected.to eq('') }

  describe 'with a starting string' do
    let(:start) { 'str1' }

    it { is_expected.to eq('str1') }

    describe 'appending a string' do
      before { buffer.append('str2') }

      it { is_expected.to eq('str1str2') }
    end

    describe 'appending a buffer' do
      let(:buffer2) { described_class.new('buf2') }

      before { buffer.append_buffer(buffer2) }

      it { is_expected.to eq('str1buf2') }

      describe 'appending a string' do
        before { buffer.append('str2') }

        it { is_expected.to eq('str1buf2str2') }
      end
    end
  end

  describe 'appending a string' do
    before { buffer.append('str2') }

    it { is_expected.to eq('str2') }
  end

  describe 'appending a buffer' do
    let(:buffer2) { described_class.new('buf2') }

    before { buffer.append_buffer(buffer2) }

    it { is_expected.to eq('buf2') }

    describe 'appending a string' do
      before { buffer.append('str2') }

      it { is_expected.to eq('buf2str2') }
    end
  end
end
